/*
 * OddEngine - a reimplementation of the Abe's Oddysee game engine
 * Copyright (C) 2015  Milan Izai <milan.izai@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Renderer.h"

#include <stdio.h>

#define WORLD_X_SCALE (640.0f/368.0f)
#define WORLD_Y_SCALE (480.0f/240.0f)

Renderer::Renderer(SDL_Window* sdlwindow, SDL_Renderer* sdlrenderer) :
    sdlwindow(sdlwindow), sdlrenderer(sdlrenderer),
    x_offset(0), y_offset(0)
{
}

Renderer::~Renderer()
{
    for (std::map<unsigned int, SDL_Texture*>::iterator it = cam_textures.begin(); it != cam_textures.end(); ++it)
        SDL_DestroyTexture(it->second);
}

void Renderer::Clear()
{
    SDL_SetRenderDrawColor(sdlrenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlrenderer);
}

void Renderer::CommitFrame()
{
    SDL_RenderPresent(sdlrenderer);
}

void Renderer::SetViewTransform(int x_offset, int y_offset)
{
    this->x_offset = x_offset;
    this->y_offset = y_offset;
}

std::pair<int, int> Renderer::MapWorldToScreen(int x, int y) const
{
    return std::make_pair<int, int>(x * WORLD_X_SCALE - x_offset, y * WORLD_Y_SCALE - y_offset);
}

std::pair<int, int> Renderer::MapScreenToWorld(int x, int y) const
{
    return std::make_pair<int, int>((x + x_offset) / WORLD_X_SCALE, (y + y_offset) / WORLD_Y_SCALE);
}

void Renderer::RegisterCam(const OddCam* cam)
{
    const Image* image = cam->GetBackgroundImage();
    SDL_Texture* texture = SDL_CreateTexture(sdlrenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
                                             image->GetWidth(), image->GetHeight());
    SDL_UpdateTexture(texture, NULL, image->GetData(), image->GetWidth() * 4);
    cam_textures[cam->GetCamId()] = texture;
}

void Renderer::UnregisterCam(const OddCam* cam)
{
    std::map<unsigned int, SDL_Texture*>::iterator it = cam_textures.find(cam->GetCamId());
    if (it != cam_textures.end()) {
        SDL_DestroyTexture(it->second);
        cam_textures.erase(it);
    }
}

void Renderer::DrawCam(const OddCam* cam, int x, int y)
{
    std::map<unsigned int, SDL_Texture*>::iterator it = cam_textures.find(cam->GetCamId());
    if (it == cam_textures.end()) {
        fprintf(stderr, "Renderer::DrawCam: cam has not been registered\n");
        return;
    }


    SDL_Rect rect;
    std::pair<int, int> screenpos = MapWorldToScreen(x, y);
    rect.x = screenpos.first;
    rect.y = screenpos.second;
    rect.w = 640;
    rect.h = 480;
    SDL_RenderCopy(sdlrenderer, it->second, NULL, &rect);
}

void Renderer::RegisterImage(const Image* image)
{
    // the game seems to ignore last row and column of the image
    SDL_Texture* texture = SDL_CreateTexture(sdlrenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
                                             image->GetWidth()-1, image->GetHeight()-1);
    SDL_UpdateTexture(texture, NULL, image->GetData(), image->GetWidth() * 4);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    image_textures[image] = texture;
}

void Renderer::UnregisterImage(const Image* image)
{
    std::map<const Image*, SDL_Texture*>::iterator it = image_textures.find(image);
    if (it != image_textures.end()) {
        SDL_DestroyTexture(it->second);
        image_textures.erase(it);
    }
}

void Renderer::DrawImage(const Image* image, int x, int y)
{
    std::map<const Image*, SDL_Texture*>::iterator it = image_textures.find(image);
    if (it == image_textures.end()) {
        fprintf(stderr, "Renderer::DrawImage: image has not been registered\n");
        return;
    }

    SDL_Rect rect;
    std::pair<int, int> screenpos = MapWorldToScreen(x, y);
    rect.x = screenpos.first;
    rect.y = screenpos.second;
    rect.w = image->GetWidth();
    rect.h = 2 * image->GetHeight();
    SDL_RenderCopy(sdlrenderer, it->second, NULL, &rect);
}

void Renderer::DBG_DrawPathCollisionItems(OddPath* path)
{
    SDL_SetRenderDrawColor(sdlrenderer, 255, 255, 255, 255);
    for (const ResPath::CollisionItem& ci : path->GetCollisionItems()) {
        SDL_RenderDrawLine(sdlrenderer,
                           WORLD_X_SCALE * ci.x1 - x_offset, WORLD_Y_SCALE * ci.y1 - y_offset,
                           WORLD_X_SCALE * ci.x2 - x_offset, WORLD_Y_SCALE * ci.y2 - y_offset);
    }
}

void Renderer::DBG_DrawPathGameObjects(OddPath* path)
{
    SDL_SetRenderDrawColor(sdlrenderer, 255, 0, 0, 255);
    for (const std::unique_ptr<Entity>& entity : path->GetEntities()) {
        SDL_Rect rect;
        rect.x = WORLD_X_SCALE * entity->x1 - x_offset;
        rect.y = WORLD_Y_SCALE * entity->y1 - y_offset;
        rect.w = (entity->x2 - entity->x1) * WORLD_X_SCALE;
        rect.h = (entity->y2 - entity->y1) * WORLD_Y_SCALE;
        SDL_RenderDrawRect(sdlrenderer, &rect);
    }
}
