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

#ifndef ENTITY_EYEORB_H
#define ENTITY_EYEORB_H

#include "Entity.h"
#include "../Animation.h"

#include <memory>

class Entity_Eyeorb : public Entity
{
public:
    Entity_Eyeorb(const OddLevel* level, const void* data, unsigned int size);

    void Think();
    void Render(Renderer* renderer);

private:
    Entity_Eyeorb(const Entity_Eyeorb&) = delete;
    Entity_Eyeorb& operator=(const Entity_Eyeorb&) = delete;

    std::unique_ptr<Anim> anim_top;
    std::unique_ptr<Anim> anim_bottom;
};

#endif
