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

#ifndef RES_PATH_H
#define RES_PATH_H

#include "Res.h"

#include <string>
#include <vector>

class ResPath : public Res
{
public:
    struct CollisionItem
    {
        int type;
        int x1, y1;
        int x2, y2;
    };
    struct GameObject
    {
        int type;
        std::vector<uint8_t> data;
    };

public:
    ResPath(const void* data, unsigned long size);

    const std::vector<std::string>& GetCameraNames() const;
    const std::vector<CollisionItem>& GetCollisionItems() const;
    const std::vector<GameObject>& GetGameObjects() const;

private:
    ResPath(const ResPath&) = delete;
    ResPath& operator=(const ResPath&) = delete;

    std::vector<std::string> camera_names;
    std::vector<CollisionItem> collision_items;
    std::vector<GameObject> game_objects;
};

#endif
