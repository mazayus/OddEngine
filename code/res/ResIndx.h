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

#ifndef RES_INDX_H
#define RES_INDX_H

#include <stdint.h>

class ResIndx
{
public:
    struct Info
    {
        uint32_t num_entries;
        uint32_t num_sectors;
        uint32_t padding1;
        uint32_t padding2;
    };
    struct Entry
    {
        char name[12];
        uint32_t first_sector;
        uint32_t num_sectors;
        uint32_t size;
    };

public:
    ResIndx(const void* data, unsigned long size);
    ~ResIndx();

    const Info* GetInfo() const;
    const Entry* GetEntry(uint32_t i) const;

private:
    ResIndx(const ResIndx&) = delete;
    ResIndx& operator=(const ResIndx&) = delete;

    Info info;
    Entry* entries;
};

#endif
