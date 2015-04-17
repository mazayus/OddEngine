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

#include "Entity_BoomMachine.h"

#include "../OddLevel.h"
#include "../Renderer.h"

#include <assert.h>

Entity_BoomMachine::Entity_BoomMachine(const OddLevel* level, const void* data, unsigned int size) :
    Entity(Entity::Type_BoomMachine, data, size)
{
    x = (x1 + x2) / 2;
    y = y1;

    const AnimSet* anim_set_button = level->FindAnimSet(0x1778);
    anim_button.reset(new Anim(anim_set_button, 0, 0));

    const AnimSet* anim_set_dispenser = level->FindAnimSet(0x1779);
    anim_dispenser.reset(new Anim(anim_set_dispenser, 0, 0));
}

void Entity_BoomMachine::Think()
{
    anim_button->Tick();
    anim_dispenser->Tick();
}

void Entity_BoomMachine::Render(Renderer* renderer)
{
    const AnimFrame* frame_button = anim_button->GetAnimFrame();
    renderer->DrawImage(frame_button->image, x + frame_button->x_offset, y + frame_button->y_offset);
    const AnimFrame* frame_dispenser = anim_dispenser->GetAnimFrame();
    renderer->DrawImage(frame_dispenser->image, x + frame_dispenser->x_offset + 30, y + frame_dispenser->y_offset - 30);
}
