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

#include "ResPath.h"
#include "../utils.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

class PathParser
{
public:
    PathParser(const uint8_t* data, uint32_t data_size);

    std::vector<std::string> camera_names;
    std::vector<ResPath::CollisionItem> collision_items;
    std::vector<ResPath::GameObject> game_objects;

private:
    void ParseCameraNames();
    void ParseCollisionItems();
    void ParseGameObjects();

    const uint8_t* data;
    uint32_t data_size;

    const uint8_t* ptr;
    uint32_t size;
};

PathParser::PathParser(const uint8_t* data, uint32_t data_size) :
    data(data), data_size(data_size), ptr(data), size(data_size)
{
    ParseCameraNames();
    ParseCollisionItems();
    ParseGameObjects();
    assert(size == camera_names.size() * 4);
}

void PathParser::ParseCameraNames()
{
    while (size >= 8) {
        if (mem_is_alnum(ptr, 8)) {
            camera_names.push_back(std::string((const char*)ptr, 8));
            ptr += 8; size -= 8;
        } else if (memcmp(ptr, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) == 0) {
            camera_names.push_back(std::string());
            ptr += 8; size -= 8;
        } else {
            // end of cam list
            break;
        }
    }
}

void PathParser::ParseCollisionItems()
{
    while (size > camera_names.size() * 4) {
        if (((*(uint16_t*)ptr == 0x0000) || (*(uint16_t*)ptr == 0x0004)) && (*(uint16_t*)(ptr+2) != 0)) {
            // end of collision info
            break;
        } else {
            ResPath::CollisionItem collision_item;
            collision_item.x1 = *(uint16_t*)ptr;
            ptr += 2; size -= 2;
            collision_item.y1 = *(uint16_t*)ptr;
            ptr += 2; size -= 2;
            collision_item.x2 = *(uint16_t*)ptr;
            ptr += 2; size -= 2;
            collision_item.y2 = *(uint16_t*)ptr;
            ptr += 2; size -= 2;
            collision_item.type = *(uint32_t*)ptr;
            ptr += 12; size -= 12;
            collision_items.push_back(collision_item);
        }
    }
}

void PathParser::ParseGameObjects()
{
    while (size > camera_names.size() * 4) {
        uint16_t end_flag = *(uint16_t*)ptr;
        ptr += 2; size -= 2;
        uint16_t obj_size = *(uint16_t*)ptr;
        ptr += 2; size -= 2;
        assert(size >= camera_names.size() * 4 + obj_size - 4);

        ResPath::GameObject game_object;

        game_object.type = *(uint32_t*)ptr;
        ptr += 4; size -= 4;

        // type specific object info
        assert(obj_size >= 8);
        for (int i = 0; i < obj_size-8; ++i) {
            game_object.data.push_back(*ptr);
            ptr += 1; size -= 1;
        }

        game_objects.push_back(game_object);
    }
}

ResPath::ResPath(const void* data, unsigned long size) :
    Res(FOURCC('P', 'a', 't', 'h'))
{
    PathParser parser((const uint8_t*) data, (uint32_t)size);
    camera_names = parser.camera_names;
    collision_items = parser.collision_items;
    game_objects = parser.game_objects;
}

const std::vector<std::string>& ResPath::GetCameraNames() const
{
    return camera_names;
}

const std::vector<ResPath::CollisionItem>& ResPath::GetCollisionItems() const
{
    return collision_items;
}

const std::vector<ResPath::GameObject>& ResPath::GetGameObjects() const
{
    return game_objects;
}
