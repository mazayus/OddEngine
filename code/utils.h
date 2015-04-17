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

#ifndef UTILS_H
#define UTILS_H

#include <assert.h>
#include <stdint.h>

#include <vector>

#define FOURCC(b0, b1, b2, b3) ((b0) | ((b1) << 8) | ((b2) << 16) | ((b3 << 24)))

uint32_t rgb565_to_argb8888(uint16_t rgb565);
uint32_t bgr555_to_argb8888(uint16_t bgr555);

uint32_t align_to_n(uint32_t number, uint8_t n);
bool mem_is_alnum(const uint8_t* ptr, int size);

std::vector<uint8_t> lzss_decompress(const uint8_t* data, uint32_t size,
                                     const uint8_t** pnewdata, uint32_t* pnewsize);

class bitstream
{
public:
    bitstream(const uint8_t* data, uint32_t size);

    uint8_t read(uint8_t num_bits);

    void skip(uint8_t num_bits);

    uint32_t get_num_remaining_bytes() const;
    uint32_t get_num_remaining_bits() const;

    uint32_t get_num_partially_read_bytes() const;
    uint32_t get_num_completely_read_bytes() const;

private:
    const uint8_t* data;
    uint32_t size;

    const uint8_t* ptr;
    uint32_t remaining_bytes;
    uint32_t remaining_bits;
};

#endif
