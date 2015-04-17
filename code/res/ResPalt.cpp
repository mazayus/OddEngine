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

#include "ResPalt.h"
#include "../utils.h"

#include <assert.h>
#include <stdint.h>

ResPalt::ResPalt(const void* data, unsigned long size) :
    Res(FOURCC('P', 'a', 'l', 't'))
{
    const uint8_t* ptr = (const uint8_t*)data;

    // header
    assert(size >= 4);
    uint32_t palette_size = *(uint32_t*)ptr;
    ptr += 4; size -= 4;

    // colors
    assert(size >= palette_size * 2);
    std::vector<uint32_t> colors;
    for (uint32_t i = 0; i < palette_size; ++i) {
        colors.push_back(bgr555_to_argb8888(*(uint16_t*)ptr));
        ptr += 2; size -= 2;
    }

    // image
    image.reset(new Image(palette_size, 1));
    uint32_t* pixels = (uint32_t*)image->GetData();
    for (uint32_t i = 0; i < palette_size; ++i)
        *pixels++ = colors.at(i);
}

const Image* ResPalt::GetImage() const
{
    return image.get();
}
