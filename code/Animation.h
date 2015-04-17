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

#ifndef ANIMATION_H
#define ANIMATION_H

#include "Image.h"

#include <memory>
#include <vector>

struct AnimFrame
{
    Image* image;
    int x_offset, y_offset;
};

struct AnimSequence
{
    int ticks_per_frame;
    std::vector<AnimFrame> frames;
};

struct AnimSet
{
    unsigned long id;
    std::vector<std::unique_ptr<Image>> images;
    std::vector<AnimSequence> sequences;
};

class Anim
{
public:
    Anim(const AnimSet* anim_set, int sequence, int frame);
    ~Anim() = default;

    void Tick();

    const AnimFrame* GetAnimFrame() const;
    const AnimSequence* GetAnimSequence() const;
    const AnimSet* GetAnimSet() const;

private:
    Anim(const Anim&) = delete;
    Anim& operator=(const Anim&) = delete;

    int ticks;
    int frame;

    const AnimFrame* anim_frame;
    const AnimSequence* anim_sequence;
    const AnimSet* anim_set;
};

#endif
