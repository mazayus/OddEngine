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

#include "Animation.h"

#include <assert.h>

Anim::Anim(const AnimSet* anim_set, int sequence, int frame) :
    ticks(0), frame(frame), anim_set(anim_set)
{
    assert(anim_set != nullptr && sequence >= 0 && frame >= 0);
    anim_sequence = &anim_set->sequences[sequence];
    anim_frame = &anim_sequence->frames[frame];
}

void Anim::Tick()
{
    ticks = (ticks + 1) % anim_sequence->ticks_per_frame;
    if (ticks == 0) {
        frame = (frame + 1) % anim_sequence->frames.size();
        anim_frame = &anim_sequence->frames[frame];
    }
}

const AnimFrame* Anim::GetAnimFrame() const
{
    return anim_frame;
}

const AnimSequence* Anim::GetAnimSequence() const
{
    return anim_sequence;
}

const AnimSet* Anim::GetAnimSet() const
{
    return anim_set;
}
