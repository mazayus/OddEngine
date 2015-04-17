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

#ifndef ODD_ARCHIVE_H
#define ODD_ARCHIVE_H

#include <string>
#include <vector>

/*
 * OddResource
 */

class OddResource
{
public:
    enum Type
    {
        Type_Anim,
        Type_Bits,
        Type_FG1,
        Type_Font,
        Type_Palt,
        Type_Path,
        Type_Plbk,
        Type_Seq,

        Type_Unknown
    };

public:
    OddResource(Type type, unsigned int id, const void* data, unsigned long size);
    ~OddResource();

    Type GetType() const;
    unsigned int GetId() const;

    const void* GetData() const;
    unsigned long GetSize() const;

private:
    OddResource(const OddResource&) = delete;
    OddResource& operator=(const OddResource&) = delete;

    Type type;
    unsigned int id;

    const void* data;
    unsigned long size;
};

/*
 * OddFile
 */

class OddFile
{
public:
    OddFile(const std::string& name, const void* data, unsigned long size);
    ~OddFile();

    std::string GetName() const;

    unsigned int GetNumResources() const;
    const OddResource* GetResource(unsigned int i) const;

private:
    OddFile(const OddFile&) = delete;
    OddFile& operator=(const OddFile&) = delete;

    std::string name;
    std::vector<OddResource*> resources;
};

/*
 * OddArchive
 */

class OddArchive
{
public:
    OddArchive(const char* filename);
    ~OddArchive();

    unsigned int GetNumFiles() const;
    const OddFile* GetFile(unsigned int i) const;
    const OddFile* GetFileByName(const std::string& name) const;

private:
    OddArchive(const OddArchive&) = delete;
    OddArchive& operator=(const OddArchive&) = delete;

    std::vector<uint8_t> data;
    std::vector<OddFile*> files;
};

#endif
