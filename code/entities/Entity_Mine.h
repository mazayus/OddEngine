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

#ifndef ENTITY_MINE_H
#define ENTITY_MINE_H

#include "Entity.h"
#include "../Animation.h"

#include <memory>

class Entity_Mine : public Entity
{
public:
    Entity_Mine(const OddLevel* level, const void* data, unsigned int size);

    void Think();
    void Render(Renderer* renderer);

private:
    Entity_Mine(const Entity_Mine&) = delete;
    Entity_Mine& operator=(const Entity_Mine&) = delete;

    std::unique_ptr<Anim> anim_main;
    std::unique_ptr<Anim> anim_lights;
};

#endif
