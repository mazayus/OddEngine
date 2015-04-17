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

#include "ResFG1.h"
#include "../utils.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

class FG1Parser
{
public:
    FG1Parser(const uint8_t* data, uint32_t data_size);

    std::vector<FG1Block> blocks;

private:
    void ParseBlockFFFF();
    void ParseBlockFFFE();
    void ParseBlockFFFD();
    void ParseBlockFFFC();
    void ParseBlock0000();

    const uint8_t* data;
    uint32_t data_size;

    const uint8_t* ptr;
    uint32_t size;

    const uint8_t* saved_ptr;
    uint32_t saved_size;

    bool in_compressed_block;
    std::vector<uint8_t> decompressed_data;
};

FG1Parser::FG1Parser(const uint8_t* data, uint32_t data_size) :
    data(data), data_size(data_size), ptr(data), size(data_size)
{
    assert(size >= 4);
    uint32_t num_blocks = *(uint32_t*)ptr;
    ptr += 4; size -= 4;

    bool found_end_block = false;
    while (!found_end_block) {
        assert(size >= 2);
        uint16_t block_type = *(uint16_t*)ptr;
        ptr += 2; size -= 2;

        switch (block_type) {
            case 0xFFFF:
                ParseBlockFFFF();
                found_end_block = true;
                break;
            case 0xFFFE:
                ParseBlockFFFE();
                break;
            case 0xFFFD:
                ParseBlockFFFD();
                break;
            case 0xFFFC:
                ParseBlockFFFC();
                break;
            case 0x0000:
                ParseBlock0000();
                break;
            default:
                assert(!"Bad block type");
                break;
        }
    }

    assert(size == 0);
}

void FG1Parser::ParseBlockFFFF()
{
    // end block
    assert(!in_compressed_block);
    assert(size >= 10);
    ptr += 10; size -= 10;
}

void FG1Parser::ParseBlockFFFE()
{
    // simple block
    assert(size >= 10);

    FG1Block block;
    block.is_custom_block = false;

    block.layer = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    block.x = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    block.y = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    block.w = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    block.h = *(uint16_t*)ptr;
    ptr += 2; size -= 2;

    blocks.push_back(block);
}

void FG1Parser::ParseBlockFFFD()
{
    // begin compressed data chunk
    assert(!in_compressed_block);

    assert(size >= 10);
    uint16_t uncompressed_size = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    uint32_t compressed_size = *(uint32_t*)ptr;
    ptr += 4; size -= 4;

    // skip padding
    ptr += 4; size -= 4;

    assert(size >= compressed_size);
    decompressed_data = lzss_decompress(ptr, size, NULL, NULL);
    ptr += compressed_size; size -= compressed_size;
    saved_ptr = ptr; saved_size = size;

    in_compressed_block = true;
    ptr = decompressed_data.data();
    size = decompressed_data.size();
}

void FG1Parser::ParseBlockFFFC()
{
    // end compressed data chunk
    assert(in_compressed_block);

    in_compressed_block = false;
    ptr = saved_ptr;
    size = saved_size;
}

void FG1Parser::ParseBlock0000()
{
    // custom block
    assert(size >= 10);

    FG1Block block;
    block.is_custom_block = true;

    block.layer = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    block.x = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    block.y = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    block.w = *(uint16_t*)ptr;
    ptr += 2; size -= 2;
    block.h = *(uint16_t*)ptr;
    ptr += 2; size -= 2;

    assert(size >= (uint32_t)(block.w * block.h * 2));
    for (int i = 0; i < block.w * block.h; ++i) {
        block.custom_data.push_back(bgr555_to_argb8888(*(uint16_t*)ptr));
        ptr += 2; size -= 2;
    }

    blocks.push_back(block);
}

ResFG1::ResFG1(const void* data, unsigned long size) :
    Res(FOURCC('F', 'G', '1', ' '))
{
    FG1Parser parser((const uint8_t*)data, (uint32_t)size);
    blocks = parser.blocks;
}

const std::vector<FG1Block>& ResFG1::GetBlocks() const
{
    return blocks;
}
