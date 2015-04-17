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

#include "OddArchive.h"
#include "res/ResIndx.h"
#include "utils.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * OddResource
 */

OddResource::OddResource(OddResource::Type type, unsigned int id, const void* data, unsigned long size) :
    type(type), id(id), data(data), size(size)
{ }

OddResource::~OddResource()
{ }

OddResource::Type OddResource::GetType() const
{
    return type;
}

unsigned int OddResource::GetId() const
{
    return id;
}

const void* OddResource::GetData() const
{
    return data;
}

unsigned long OddResource::GetSize() const
{
    return size;
}

/*
 * OddFile
 */

OddFile::OddFile(const std::string& name, const void* data, unsigned long size) :
    name(name)
{
    const uint8_t* ptr = (const uint8_t*)data;

    // TODO: VH and VB files don't follow the structure of other files
    if (name.find(".VH") != name.npos || name.find(".VB") != name.npos)
        return;

    while (size != 0) {

        // header
        assert(size >= 16);
        uint32_t length = *(uint32_t*)ptr;
        ptr += 8; size -= 8;
        uint32_t magic = *(uint32_t*)ptr;
        ptr += 4; size -= 4;
        uint32_t id = *(uint32_t*)ptr;
        ptr += 4; size -= 4;

        if (magic == FOURCC('E', 'n', 'd', '!'))
            break;

        assert(length >= 16);
        assert(length - 16 <= size);

        switch (magic) {
            case FOURCC('A', 'n', 'i', 'm'):
                resources.push_back(new OddResource(OddResource::Type_Anim, id, ptr, length - 16));
                break;
            case FOURCC('B', 'i', 't', 's'):
                resources.push_back(new OddResource(OddResource::Type_Bits, id, ptr, length - 16));
                break;
            case FOURCC('F', 'G', '1', ' '):
                resources.push_back(new OddResource(OddResource::Type_FG1, id, ptr, length - 16));
                break;
            case FOURCC('F', 'o', 'n', 't'):
                resources.push_back(new OddResource(OddResource::Type_Font, id, ptr, length - 16));
                break;
            case FOURCC('P', 'a', 'l', 't'):
                resources.push_back(new OddResource(OddResource::Type_Palt, id, ptr, length - 16));
                break;
            case FOURCC('P', 'a', 't', 'h'):
                resources.push_back(new OddResource(OddResource::Type_Path, id, ptr, length - 16));
                break;
            case FOURCC('P', 'l', 'b', 'k'):
                resources.push_back(new OddResource(OddResource::Type_Plbk, id, ptr, length - 16));
                break;
            case FOURCC('S', 'e', 'q', ' '):
                resources.push_back(new OddResource(OddResource::Type_Seq, id, ptr, length - 16));
                break;
            default:
                resources.push_back(new OddResource(OddResource::Type_Unknown, id, ptr, length - 16));
                break;
        }

        ptr += length - 16;
        size -= length - 16;
    }
}

OddFile::~OddFile()
{
    for (OddResource* resource : resources)
        delete resource;
}

std::string OddFile::GetName() const
{
    return name;
}

unsigned int OddFile::GetNumResources() const
{
    return resources.size();
}

const OddResource* OddFile::GetResource(unsigned int i) const
{
    return resources.at(i);
}

/*
 * OddArchive
 */

OddArchive::OddArchive(const char* filename)
{
    // read file into a buffer
    FILE* fp = fopen(filename, "rb");
    assert(fp != NULL);

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    assert(size > 0);

    data.resize(size);
    fread(data.data(), 1, size, fp);
    assert(!feof(fp) && !ferror(fp));

    fclose(fp);

    const uint8_t* ptr = data.data();

    // header
    assert(size >= 16);

    uint32_t length = *(uint32_t*)ptr;
    assert(length >= 16);
    ptr += 8; size -= 8;

    uint32_t magic = *(uint32_t*)ptr;
    assert(magic == FOURCC('I', 'n', 'd', 'x'));
    ptr += 8; size -= 8;

    // indx
    ResIndx indx(ptr, length - 16);

    // files
    for (uint32_t i = 0; i < indx.GetInfo()->num_entries; ++i) {
        std::string name(indx.GetEntry(i)->name, 12);
        unsigned long null_pos = name.find('\0');
        if (null_pos != name.npos)
            name.erase(null_pos);

        uint32_t file_offset = indx.GetEntry(i)->first_sector * 2048;
        uint32_t file_size = indx.GetEntry(i)->size;
        files.push_back(new OddFile(name, data.data() + file_offset, file_size));
    }
}

OddArchive::~OddArchive()
{
    for (OddFile* file : files)
        delete file;
}

unsigned int OddArchive::GetNumFiles() const
{
    return files.size();
}

const OddFile* OddArchive::GetFile(unsigned int i) const
{
    return files.at(i);
}

const OddFile* OddArchive::GetFileByName(const std::string& name) const
{
    for (const OddFile* file : files)
        if (file->GetName() == name)
            return file;
    return nullptr;
}
