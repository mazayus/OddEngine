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

#include "utils.h"

#include <assert.h>
#include <ctype.h>

uint32_t rgb565_to_argb8888(uint16_t rgb565)
{
    uint8_t r = 255.0f / 31.0f * ((rgb565 >> 11) & 0x1F);
    uint8_t g = 255.0f / 63.0f * ((rgb565 >> 5) & 0x3F);
    uint8_t b = 255.0f / 31.0f * (rgb565 & 0x1F);
    uint8_t a = 255;
    return (a << 24) | (r << 16) | (g << 8) | b;
}

uint32_t bgr555_to_argb8888(uint16_t bgr555)
{
    uint8_t b = 255.0f / 31.0f * ((bgr555 >> 10) & 0x1F);
    uint8_t g = 255.0f / 31.0f * ((bgr555 >> 5) & 0x1F);
    uint8_t r = 255.0f / 31.0f * (bgr555 & 0x1F);
    uint8_t a = (bgr555 == 0x0000) ? 0 : 255;
    return (a << 24) | (r << 16) | (g << 8) | b;
}

uint32_t align_to_n(uint32_t number, uint8_t n)
{
    if (number % n != 0)
        return number + (n - number % n);
    else
        return number;
}

bool mem_is_alnum(const uint8_t* ptr, int size)
{
    while (size > 0) {
        if (!isalnum(*ptr))
            return false;
        ++ptr;
        --size;
    }
    return true;
}

std::vector<uint8_t> lzss_decompress(const uint8_t* data, uint32_t size,
                                     const uint8_t** pnewdata, uint32_t* pnewsize)
{
    std::vector<uint8_t> decompressed_data;

    assert(size >= 4);
    uint32_t decompressed_size = *(const uint32_t*)data;
    data += 4; size -= 4;

    while (decompressed_data.size() != decompressed_size) {
        assert(decompressed_data.size() < decompressed_size);

        assert(size >= 1);
        uint8_t byte = *data;
        data += 1; size -= 1;

        if (byte & 0x80) {

            assert(size >= 1);
            uint8_t next_byte = *data;
            data += 1; size -= 1;

            uint16_t length = ((byte >> 2) & 0x1F) + 3;
            uint16_t offset = (((byte << 8) | next_byte) & 0x3FF) + 1;
            for (uint16_t i = 0; i < length; ++i) {
                assert(decompressed_data.size() < decompressed_size);
                uint8_t value = decompressed_data.at(decompressed_data.size() - offset);
                decompressed_data.push_back(value);
            }
        } else {

            uint16_t length = (byte & 0x7F) + 1;
            for (uint16_t i = 0; i < length; ++i) {
                assert(decompressed_data.size() < decompressed_size);

                assert(size >= 1);
                uint8_t value = *data;
                data += 1; size -= 1;

                decompressed_data.push_back(value);
            }
        }
    }

    if (pnewdata)
        *pnewdata = data;
    if (pnewsize)
        *pnewsize = size;

    return decompressed_data;
}

bitstream::bitstream(const uint8_t* data, uint32_t size) :
    data(data), size(size), ptr(data), remaining_bytes(size), remaining_bits(8)
{ }

uint8_t bitstream::read(uint8_t num_bits)
{
    assert(num_bits > 0 && num_bits <= 8);

    uint8_t mask = 0;
    for (int i = 0; i < num_bits; ++i)
        mask |= (1 << i);

    uint8_t value = 0;
    if (num_bits <= remaining_bits) {
        assert(remaining_bytes > 0);
        uint8_t byte = *ptr;
        value = (byte >> (8 - remaining_bits)) & mask;
    } else {
        assert(remaining_bytes > 1);
        uint16_t word = (*ptr) | (*(ptr+1) << 8);
        value = (word >> (8 - remaining_bits)) & mask;
    }
    skip(num_bits);
    return value;
}

void bitstream::skip(uint8_t num_bits)
{
    assert(num_bits > 0 && num_bits <= 8);

    if (num_bits <= remaining_bits) {
        assert(remaining_bytes > 0);
        remaining_bits -= num_bits;
        if (remaining_bits == 0) {
            remaining_bits = 8;
            --remaining_bytes;
            ++ptr;
        }
    } else {
        assert(remaining_bytes > 1);
        remaining_bits = 8 - (num_bits - remaining_bits);
        --remaining_bytes;
        ++ptr;
    }
}

uint32_t bitstream::get_num_remaining_bytes() const
{
    return remaining_bytes;
}

uint32_t bitstream::get_num_remaining_bits() const
{
    return remaining_bits;
}

uint32_t bitstream::get_num_partially_read_bytes() const
{
    return size - remaining_bytes + (remaining_bits == 8 ? 0 : 1);
}

uint32_t bitstream::get_num_completely_read_bytes() const
{
    return size - remaining_bytes;
}
