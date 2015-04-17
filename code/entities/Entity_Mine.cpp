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

#include "Entity_Mine.h"

#include "../OddLevel.h"
#include "../Renderer.h"

#include <assert.h>

Entity_Mine::Entity_Mine(const OddLevel* level, const void* data, unsigned int size) :
    Entity(Entity::Type_Mine, data, size)
{
    x = (x1 + x2) / 2;
    y = y2;

    const AnimSet* anim_set_main = level->FindAnimSet(0x040C);
    anim_main.reset(new Anim(anim_set_main, 0, 0));

    const AnimSet* anim_set_lights = level->FindAnimSet(0x0410);
    anim_lights.reset(new Anim(anim_set_lights, 0, 0));
}

void Entity_Mine::Think()
{
    anim_main->Tick();
    anim_lights->Tick();
}

void Entity_Mine::Render(Renderer* renderer)
{
    const AnimFrame* frame_main = anim_main->GetAnimFrame();
    renderer->DrawImage(frame_main->image, x + frame_main->x_offset, y + frame_main->y_offset);
    const AnimFrame* frame_lights = anim_lights->GetAnimFrame();
    renderer->DrawImage(frame_lights->image, x + frame_lights->x_offset, y + frame_lights->y_offset);
}
