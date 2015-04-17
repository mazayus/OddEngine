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

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

#include "Game.h"
#include "OddArchive.h"
#include "OddLevel.h"
#include "Renderer.h"

#define WINDOW_W 1366
#define WINDOW_H 768

#define WORLD_X_SCALE (640.0f/368.0f)
#define WORLD_Y_SCALE (480.0f/240.0f)

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: OddEngine LEVEL-ID\n");
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* sdlwindow = SDL_CreateWindow("OddEngine", 0, 0, WINDOW_W, WINDOW_H, 0);
    if (!sdlwindow) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* sdlrenderer = SDL_CreateRenderer(sdlwindow, -1, SDL_RENDERER_SOFTWARE);
    if (!sdlrenderer) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Game game("aodata");
    Renderer renderer(sdlwindow, sdlrenderer);

    std::unique_ptr<OddLevel> level = game.LoadLevel(argv[1]);
    for (const std::unique_ptr<OddPath>& path : level->GetPaths())
        for (const std::unique_ptr<OddCam>& cam : path->GetCams())
            if (cam)
                renderer.RegisterCam(cam.get());
    for (const std::unique_ptr<AnimSet>& anim_set : level->AnimSets())
        for (const std::unique_ptr<Image>& image : anim_set->images)
            renderer.RegisterImage(image.get());

    bool quit = false;
    while (!quit) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT: level->GetActivePath()->ActivateLeftCam(); break;
                    case SDLK_RIGHT: level->GetActivePath()->ActivateRightCam(); break;
                    case SDLK_UP: level->GetActivePath()->ActivateTopCam(); break;
                    case SDLK_DOWN: level->GetActivePath()->ActivateBottomCam(); break;
                    case SDLK_LEFTBRACKET: level->ActivatePreviousPath(); break;
                    case SDLK_RIGHTBRACKET: level->ActivateNextPath(); break;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                std::pair<int, int> worldpos = renderer.MapScreenToWorld(event.button.x, event.button.y);
                int x = worldpos.first, y = worldpos.second;
                for (const std::unique_ptr<Entity>& entity : level->GetActivePath()->GetEntities()) {
                    if (x >= entity->x1 && x <= entity->x2 && y >= entity->y1 && y <= entity->y2) {
                        printf("Object(type=%d, x1=%d, y1=%d, x2=%d, y2=%d)\n",
                               (int)entity->GetType(), (int)entity->x1, (int)entity->y1, (int)entity->x2, (int)entity->y2);
                        fflush(stdout);
                    }
                }
            }
        }

        level->GetActivePath()->Update();

        float cam_x_offset = 256 + 1024 * level->GetActivePath()->GetCurrentGridColumn();
        float cam_y_offset = 116 + 480 * level->GetActivePath()->GetCurrentGridRow();
        float view_x_offset = cam_x_offset * WORLD_X_SCALE - (WINDOW_W - 640) / 2;
        float view_y_offset = cam_y_offset * WORLD_Y_SCALE - (WINDOW_H - 480) / 2;
        renderer.SetViewTransform(view_x_offset, view_y_offset);

        renderer.Clear();

        if (level->GetActivePath()->GetActiveCam())
            renderer.DrawCam(level->GetActivePath()->GetActiveCam(), cam_x_offset, cam_y_offset);
        level->GetActivePath()->Render(&renderer);

        renderer.DBG_DrawPathCollisionItems(level->GetActivePath());
        renderer.DBG_DrawPathGameObjects(level->GetActivePath());

        renderer.CommitFrame();

        SDL_Delay(33);
    }

    SDL_Quit();
    return 0;
}
