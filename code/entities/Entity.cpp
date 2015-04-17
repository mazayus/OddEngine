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

#include "Entity.h"
#include "Entity_BackgroundAnim.h"
#include "Entity_Bomb.h"
#include "Entity_BoomMachine.h"
#include "Entity_ElectricWall.h"
#include "Entity_Eyeorb.h"
#include "Entity_Lever.h"
#include "Entity_Mamorb.h"
#include "Entity_MeatSaw.h"
#include "Entity_Mine.h"
#include "Entity_MotionDetector.h"
#include "Entity_Mudokon.h"
#include "Entity_Pullring.h"
#include "Entity_Scrab.h"
#include "Entity_Slig.h"
#include "Entity_Slog.h"
#include "Entity_TimeBomb.h"
#include "Entity_Trapdoor.h"

#include "../OddLevel.h"

#include <assert.h>
#include <stdint.h>

Entity* Entity::Create(const OddLevel* level, Entity::Type type, const void* data, unsigned int size)
{
    switch (type) {
        case Entity::Type_BackgroundAnim:
            return new Entity_BackgroundAnim(level, data, size);
        case Entity::Type_Bomb:
            return new Entity_Bomb(level, data, size);
        case Entity::Type_BoomMachine:
            return new Entity_BoomMachine(level, data, size);
        case Entity::Type_ElectricWall:
            return new Entity_ElectricWall(level, data, size);
        case Entity::Type_Eyeorb:
            return new Entity_Eyeorb(level, data, size);
        case Entity::Type_Lever:
            return new Entity_Lever(level, data, size);
        case Entity::Type_Mamorb:
            return new Entity_Mamorb(level, data, size);
        case Entity::Type_MeatSaw:
            return new Entity_MeatSaw(level, data, size);
        case Entity::Type_Mine:
            return new Entity_Mine(level, data, size);
        case Entity::Type_MotionDetector:
            return new Entity_MotionDetector(level, data, size);
        case Entity::Type_Mudokon:
            return new Entity_Mudokon(level, data, size);
        case Entity::Type_Pullring:
            return new Entity_Pullring(level, data, size);
        case Entity::Type_Scrab:
            return new Entity_Scrab(level, data, size);
        case Entity::Type_Slig:
            return new Entity_Slig(level, data, size);
        case Entity::Type_Slog:
            return new Entity_Slog(level, data, size);
        case Entity::Type_TimeBomb:
            return new Entity_TimeBomb(level, data, size);
        case Entity::Type_Trapdoor:
            return new Entity_Trapdoor(level, data, size);
        default:
            return new Entity(type, data, size);
    }
}

Entity::Entity(Entity::Type type, const void* data, unsigned int size) :
    x(0), y(0), x1(0), y1(0), x2(0), y2(0), type(type)
{
    assert(size >= 16);
    const uint8_t* ptr = (const uint8_t*)data;

    // padding
    ptr += 4; size -= 4;

    // x0, y0
    x = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    y = *(uint16_t*)ptr;
    ptr += 2; size -= 2;

    // x1, y1
    x1 = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    y1 = *(uint16_t*)ptr;
    ptr += 2; size -= 2;

    // x2, y2
    x2 = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    y2 = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
}

Entity::Type Entity::GetType()
{
    return type;
}

void Entity::Think()
{

}

void Entity::Render(Renderer* renderer)
{

}
