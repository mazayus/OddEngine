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

#include "ResIndx.h"

#include <assert.h>
#include <string.h>

ResIndx::ResIndx(const void* data, unsigned long size)
{
    const uint8_t* ptr = (const uint8_t*)data;

    // info
    assert(size >= 16);
    info.num_entries = *(uint32_t*)ptr;
    ptr += 4; size -= 4;
    info.num_sectors = *(uint32_t*)ptr;
    ptr += 4; size -= 4;
    info.padding1 = *(uint32_t*)ptr;
    ptr += 4; size -= 4;
    info.padding2 = *(uint32_t*)ptr;
    ptr += 4; size -= 4;

    // entries
    assert(size >= 24 * info.num_entries);
    entries = new Entry[info.num_entries];
    for (uint32_t i = 0; i < info.num_entries; ++i) {
        memcpy(entries[i].name, ptr, 12);
        ptr += 12; size -= 12;
        entries[i].first_sector = *(uint32_t*)ptr;
        ptr += 4; size -= 4;
        entries[i].num_sectors = *(uint32_t*)ptr;
        ptr += 4; size -= 4;
        entries[i].size = *(uint32_t*)ptr;
        ptr += 4; size -= 4;
    }
}

ResIndx::~ResIndx()
{
    delete[] entries;
}

const ResIndx::Info* ResIndx::GetInfo() const
{
    return &info;
}

const ResIndx::Entry* ResIndx::GetEntry(uint32_t i) const
{
    return entries + i;
}
