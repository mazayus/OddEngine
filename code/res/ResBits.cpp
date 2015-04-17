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

#include "ResBits.h"
#include "../utils.h"

#include <assert.h>
#include <stdint.h>

ResBits::ResBits(const void* data, unsigned long size) :
    Res(FOURCC('B', 'i', 't', 's'))
{
    const int num_stripes = 40;
    const int stripe_width = 16;
    const int stripe_height = 240;

    const uint8_t* ptr = (const uint8_t*) data;

    image.reset(new Image(640, 240));
    uint32_t* pixels = (uint32_t*)image->GetData();

    for (int i = 0; i < num_stripes; ++i) {
        // stripe size
        assert(size >= 2);
        uint16_t stripe_size = *(uint16_t*)ptr;
        assert(stripe_size == stripe_width * stripe_height * 2);
        ptr += 2; size -= 2;
        // stripe pixels
        assert(size >= stripe_size);
        for (int y = 0; y < stripe_height; ++y) {
            for (int x = 0; x < stripe_width; ++x) {
                pixels[y * 640 + i * stripe_width + x] = rgb565_to_argb8888(*(uint16_t*)ptr);
                ptr += 2; size -= 2;
            }
        }
    }
}

const Image* ResBits::GetImage() const
{
    return image.get();
}
