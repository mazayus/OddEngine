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

#ifndef RES_FG1_H
#define RES_FG1_H

#include "Res.h"

#include <vector>

struct FG1Block
{
    int layer;
    int x, y, w, h;

    bool is_custom_block;
    std::vector<uint32_t> custom_data;
};

class ResFG1 : public Res
{
public:
    ResFG1(const void* data, unsigned long size);

    const std::vector<FG1Block>& GetBlocks() const;

private:
    ResFG1(const ResFG1&) = delete;
    ResFG1& operator=(const ResFG1&) = delete;

    std::vector<FG1Block> blocks;
};

#endif
