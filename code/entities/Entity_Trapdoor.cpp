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

#include "Entity_Trapdoor.h"

#include "../OddLevel.h"
#include "../Renderer.h"

#include <assert.h>

Entity_Trapdoor::Entity_Trapdoor(const OddLevel* level, const void* data, unsigned int size) :
    Entity(Entity::Type_Trapdoor, data, size)
{
    x = (x1 + x2) / 2;
    y = y2 + 50;

    const AnimSet* anim_set = level->FindAnimSet(0x03EC);
    anim.reset(new Anim(anim_set, 0, 0));
}

void Entity_Trapdoor::Think()
{
    anim->Tick();
}

void Entity_Trapdoor::Render(Renderer* renderer)
{
    const AnimFrame* frame = anim->GetAnimFrame();
    renderer->DrawImage(frame->image, x + frame->x_offset, y + frame->y_offset);
}
