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

#ifndef IMAGE_H
#define IMAGE_H

class Image
{
public:
    Image(unsigned int width, unsigned int height);
    ~Image();

    void* GetData();
    const void* GetData() const;

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    void Resize(unsigned int new_width, unsigned int new_height);

private:
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    char* data;
    unsigned int width;
    unsigned int height;
};

#endif
