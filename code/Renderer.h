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

#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

#include <map>

#include "Image.h"
#include "OddLevel.h"

class Renderer
{
public:
    Renderer(SDL_Window* sdlwindow, SDL_Renderer* sdlrenderer);
    ~Renderer();

    void Clear();
    void CommitFrame();

    void SetViewTransform(int x_offset, int y_offset);

    std::pair<int, int> MapWorldToScreen(int x, int y) const;
    std::pair<int, int> MapScreenToWorld(int x, int y) const;

    void RegisterCam(const OddCam* cam);
    void UnregisterCam(const OddCam* cam);
    void DrawCam(const OddCam* cam, int x, int y);

    void RegisterImage(const Image* image);
    void UnregisterImage(const Image* image);
    void DrawImage(const Image* image, int x, int y);

    void DBG_DrawPathCollisionItems(OddPath* path);
    void DBG_DrawPathGameObjects(OddPath* path);

private:
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    SDL_Window* sdlwindow;
    SDL_Renderer* sdlrenderer;

    int x_offset, y_offset;

    std::map<unsigned int, SDL_Texture*> cam_textures;
    std::map<const Image*, SDL_Texture*> image_textures;
};

#endif
