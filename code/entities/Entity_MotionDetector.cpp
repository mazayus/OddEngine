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

#include "Entity_MotionDetector.h"

#include "../OddLevel.h"
#include "../Renderer.h"

#include <assert.h>

Entity_MotionDetector::Entity_MotionDetector(const OddLevel* level, const void* data, unsigned int size) :
    Entity(Entity::Type_MotionDetector, data, size)
{
    x = (x1 + x2) / 2;
    y = y2;

    const AnimSet* anim_set = level->FindAnimSet(0x1771);
    anim.reset(new Anim(anim_set, 0, 0));
}

void Entity_MotionDetector::Think()
{
    anim->Tick();
}

void Entity_MotionDetector::Render(Renderer* renderer)
{
    const AnimFrame* frame = anim->GetAnimFrame();
    renderer->DrawImage(frame->image, x + frame->x_offset, y + frame->y_offset);
}
