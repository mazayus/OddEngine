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

#include "Entity_Eyeorb.h"

#include "../OddLevel.h"
#include "../Renderer.h"

#include <assert.h>

Entity_Eyeorb::Entity_Eyeorb(const OddLevel* level, const void* data, unsigned int size) :
    Entity(Entity::Type_Eyeorb, data, size)
{
    x = (x1 + x2) / 2;
    y = y1;

    const AnimSet* anim_set = level->FindAnimSet(0x07D8);
    anim_top.reset(new Anim(anim_set, 2, 0));
    anim_bottom.reset(new Anim(anim_set, 0, 0));
}

void Entity_Eyeorb::Think()
{
    anim_top->Tick();
    anim_bottom->Tick();
}

void Entity_Eyeorb::Render(Renderer* renderer)
{
    const AnimFrame* frame_top = anim_top->GetAnimFrame();
    renderer->DrawImage(frame_top->image, x + frame_top->x_offset, y + frame_top->y_offset);
    const AnimFrame* frame_bottom = anim_bottom->GetAnimFrame();
    renderer->DrawImage(frame_bottom->image, x + frame_bottom->x_offset, y + frame_bottom->y_offset);
}
