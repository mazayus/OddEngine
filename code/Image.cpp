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

#include "Image.h"

#include <stdio.h>
#include <stdlib.h>

Image::Image(unsigned int width, unsigned int height) :
    data(nullptr), width(width), height(height)
{
    data = new char[width * height * 4];
}

Image::~Image()
{
    delete[] data;
}

void* Image::GetData()
{
    return data;
}

const void* Image::GetData() const
{
    return data;
}

unsigned int Image::GetWidth() const
{
    return width;
}

unsigned int Image::GetHeight() const
{
    return height;
}

void Image::Resize(unsigned int new_width, unsigned int new_height)
{
    char* new_data = new char[new_width * new_height * 4];

    unsigned int w = (width < new_width) ? width : new_width;
    unsigned int h = (height < new_height) ? height : new_height;
    for (unsigned int y = 0; y < h; ++y) {
        for (unsigned int x = 0; x < w; ++x) {
            new_data[(y * new_width + x) * 4 + 0] = data[(y * width + x) * 4 + 0];
            new_data[(y * new_width + x) * 4 + 1] = data[(y * width + x) * 4 + 1];
            new_data[(y * new_width + x) * 4 + 2] = data[(y * width + x) * 4 + 2];
            new_data[(y * new_width + x) * 4 + 3] = data[(y * width + x) * 4 + 3];
        }
    }

    delete[] data;
    data = new_data;

    width = new_width;
    height = new_height;
}
