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

#include "ResFont.h"
#include "../utils.h"

#include <assert.h>
#include <stdint.h>

ResFont::ResFont(const void* data, unsigned long size) :
    Res(FOURCC('F', 'o', 'n', 't'))
{
    const uint8_t* ptr = (const uint8_t*)data;

    // header
    assert(size >= 8);
    uint16_t width = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    uint16_t height = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    uint16_t unknown = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    uint16_t palette_size = *(uint16_t*)ptr;
    ptr += 2; size -= 2;

    // palette
    assert(size >= (uint32_t)palette_size * 2);
    std::vector<uint32_t> palette;
    for (unsigned int i = 0; i < palette_size; ++i) {
        palette.push_back(bgr555_to_argb8888(*(uint16_t*)ptr));
        ptr += 2; size -= 2;
    }

    // pixels
    assert(size == (unsigned long)(width * height / 2));
    image.reset(new Image(width, height));
    uint32_t* pixels = (uint32_t*)image->GetData();
    // TODO: handle the case when width is odd
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; x += 2) {
            *pixels++ = palette.at((*ptr)&0xF);
            *pixels++ = palette.at(((*ptr)>>4)&0xF);
            ptr += 1; size -= 1;
        }
    }
}

const Image* ResFont::GetImage() const
{
    return image.get();
}
