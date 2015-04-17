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

#include "ResAnim.h"
#include "../utils.h"

#include <assert.h>
#include <stdint.h>

#include <algorithm>

class AnimParser
{
public:
    AnimParser(const uint8_t* data, uint32_t data_size);

    std::vector<uint32_t> palette;
    std::vector<std::unique_ptr<Image>> images;

    struct FrameInfo {
        uint32_t frame_offset;
        uint32_t magic;
        int16_t x1, y1;
        int16_t x2, y2;
    };
    std::vector<FrameInfo> frame_infos;
    unsigned int FindFrameByOffset(uint32_t offset);

    struct SequenceInfo {
        uint16_t ticks_per_frame;
        uint16_t num_frames;
        uint16_t unknown1;
        uint16_t unknown2; // always in the range [0..3]
        std::vector<uint32_t> frame_info_offsets;
    };
    std::vector<SequenceInfo > sequence_infos;
    unsigned int FindFrameInfoByOffset(uint32_t offset);

private:
    void ParseHeader();
    void ParsePalette();
    void ParseFrames();
    void ParseFrameInfos();
    void ParseFrameSequences();

    std::unique_ptr<Image> ParseFrame0800(uint8_t frame_width, uint8_t frame_height);
    std::unique_ptr<Image> ParseFrame0400(uint8_t frame_width, uint8_t frame_height);
    std::unique_ptr<Image> ParseFrame0804(uint8_t frame_width, uint8_t frame_height);
    std::unique_ptr<Image> ParseFrame0404(uint8_t frame_width, uint8_t frame_height);
    std::unique_ptr<Image> ParseFrame0802(uint8_t frame_width, uint8_t frame_height);
    std::unique_ptr<Image> ParseFrame0803(uint8_t frame_width, uint8_t frame_height);

    void ParseFrameInfo00000003(FrameInfo* info);
    void ParseFrameInfo00000005(FrameInfo* info);
    void ParseFrameInfo00010003(FrameInfo* info);

    const uint8_t* data;
    uint32_t data_size;

    const uint8_t* ptr;
    uint32_t size;

    std::vector<uint32_t> frame_offsets;
    std::vector<uint32_t> frame_info_offsets;
};

AnimParser::AnimParser(const uint8_t* data, uint32_t data_size) :
    data(data), data_size(data_size), ptr(data), size(data_size)
{
    ParseHeader();
    ParsePalette();
    ParseFrames();
    ParseFrameInfos();
    ParseFrameSequences();
    assert(size == 0);
}

unsigned int AnimParser::FindFrameByOffset(uint32_t offset)
{
    return std::find(frame_offsets.begin(), frame_offsets.end(), offset) - frame_offsets.begin();
}

unsigned int AnimParser::FindFrameInfoByOffset(uint32_t offset)
{
    return std::find(frame_info_offsets.begin(), frame_info_offsets.end(), offset) - frame_info_offsets.begin();
}

void AnimParser::ParseHeader()
{
    assert(size >= 8);
    ptr += 2; size -= 2; // width
    ptr += 2; size -= 2; // height
    ptr += 4; size -= 4; // last frame sequence offset
}

void AnimParser::ParsePalette()
{
    assert(size >= 4);
    uint32_t num_palette_colors = *(uint32_t*)ptr;
    ptr += 4; size -= 4;

    assert(size >= num_palette_colors * 2);
    for (uint32_t i = 0; i < num_palette_colors; ++i) {
        palette.push_back(bgr555_to_argb8888(*(uint16_t*)ptr));
        ptr += 2; size -= 2;
    }
}

void AnimParser::ParseFrames()
{
    while (size >= 8) {
        uint32_t frame_magic = *(uint32_t*)ptr;
        if (frame_magic != 0x08)
            break;
        frame_offsets.push_back(ptr - (const uint8_t*)data);
        ptr += 4; size -= 4;
        uint8_t frame_width = *ptr;
        ptr += 1; size -= 1;
        uint8_t frame_height = *ptr;
        ptr += 1; size -= 1;
        uint8_t type1 = *ptr;
        ptr += 1; size -= 1;
        uint8_t type2 = *ptr;
        ptr += 1; size -= 1;

        std::unique_ptr<Image> image;

        switch ((type1 << 8) | type2) {
            case 0x0800:
                image = ParseFrame0800(frame_width, frame_height);
                break;
            case 0x0400:
                image = ParseFrame0400(frame_width, frame_height);
                break;
            case 0x0804:
                image = ParseFrame0804(frame_width, frame_height);
                break;
            case 0x0404:
                image = ParseFrame0404(frame_width, frame_height);
                break;
            case 0x0802:
                image = ParseFrame0802(frame_width, frame_height);
                break;
            case 0x0803:
                image = ParseFrame0803(frame_width, frame_height);
                break;
            default:
                assert(!"Bad frame type");
                break;
        }

        if (image) {
            if (image->GetWidth() != frame_width || image->GetHeight() != frame_height)
                image->Resize(frame_width, frame_height);
            images.push_back(std::move(image));
        }
    }
}

std::unique_ptr<Image> AnimParser::ParseFrame0800(uint8_t frame_width, uint8_t frame_height)
{
    frame_width = align_to_n(frame_width, 4);
    assert(size >= frame_width * frame_height);

    std::unique_ptr<Image> image(new Image(frame_width, frame_height));
    uint32_t* pixels = (uint32_t*)image->GetData();

    for (int i = 0; i < frame_width * frame_height; ++i) {
        *pixels++ = palette.at(*ptr);
        ptr += 1; size -= 1;
    }

    return image;
}

std::unique_ptr<Image> AnimParser::ParseFrame0400(uint8_t frame_width, uint8_t frame_height)
{
    frame_width = align_to_n(frame_width, 8);
    assert(size >= frame_width * frame_height / 2);

    std::unique_ptr<Image> image(new Image(frame_width, frame_height));
    uint32_t* pixels = (uint32_t*)image->GetData();
    for (int i = 0; i < frame_width * frame_height / 2; ++i) {
        *pixels++ = palette.at(*ptr & 0xF);
        *pixels++ = palette.at((*ptr >> 4) & 0xF);
        ptr += 1; size -= 1;
    }

    return image;
}

std::unique_ptr<Image> AnimParser::ParseFrame0804(uint8_t frame_width, uint8_t frame_height)
{
    frame_width = align_to_n(frame_width, 4);

    const uint8_t* newptr = ptr;
    uint32_t newsize = size;
    std::vector<uint8_t> decompressed_data = lzss_decompress(ptr, size, &newptr, &newsize);
    uint32_t compressed_size = size - newsize;
    ptr = newptr + (align_to_n(compressed_size, 4) - compressed_size);
    size = newsize - (align_to_n(compressed_size, 4) - compressed_size);

    std::unique_ptr<Image> image(new Image(frame_width, frame_height));
    uint32_t* pixels = (uint32_t*)image->GetData();
    for (int i = 0; i < frame_width * frame_height; ++i) {
        *pixels++ = palette.at(decompressed_data.at(i));
    }

    return image;
}

std::unique_ptr<Image> AnimParser::ParseFrame0404(uint8_t frame_width, uint8_t frame_height)
{
    frame_width = align_to_n(frame_width, 8);

    const uint8_t* newptr = ptr;
    uint32_t newsize = size;
    std::vector<uint8_t> decompressed_data = lzss_decompress(ptr, size, &newptr, &newsize);
    uint32_t compressed_size = size - newsize;
    ptr = newptr + (align_to_n(compressed_size, 4) - compressed_size);
    size = newsize - (align_to_n(compressed_size, 4) - compressed_size);

    std::unique_ptr<Image> image(new Image(frame_width, frame_height));
    uint32_t* pixels = (uint32_t*)image->GetData();
    for (int i = 0; i < frame_width * frame_height / 2; ++i) {
        *pixels++ = palette.at(decompressed_data.at(i) & 0xF);
        *pixels++ = palette.at((decompressed_data.at(i) >> 4) & 0xF);
    }

    return image;
}

std::unique_ptr<Image> AnimParser::ParseFrame0802(uint8_t frame_width, uint8_t frame_height)
{
    frame_width = align_to_n(frame_width, 4);

    assert(size >= 4);
    uint32_t num_ops = *(uint32_t*)ptr;
    ptr += 4; size -= 4;
    uint32_t num_bytes = (num_ops * 6 + (8 - 1)) / 8;
    assert(size >= num_bytes);

    std::unique_ptr<Image> image(new Image(frame_width, frame_height));
    uint32_t* pixels = (uint32_t*)image->GetData();

    bitstream stream(ptr, num_bytes);

    for (uint32_t i = 0; i < num_ops; ++i) {
        uint8_t op = stream.read(6);
        *pixels++ = palette.at(op);
    }

    ptr += align_to_n(num_bytes, 4);
    size -= align_to_n(num_bytes, 4);

    return image;
}

std::unique_ptr<Image> AnimParser::ParseFrame0803(uint8_t frame_width, uint8_t frame_height)
{
    frame_width = align_to_n(frame_width, 4);

    assert(size >= 4);
    uint32_t num_ops = *(uint32_t*)ptr;
    ptr += 4; size -= 4;
    uint32_t num_bytes = (num_ops * 6 + (8 - 1)) / 8;
    assert(size >= num_bytes);

    std::unique_ptr<Image> image(new Image(frame_width, frame_height));
    uint32_t* pixels = (uint32_t*)image->GetData();

    bitstream stream(ptr, num_bytes);

    uint32_t num_pixels = 0;
    int remaining_ops = 0;
    for (uint32_t i = 0; i < num_ops; ++i) {
        uint8_t op = stream.read(6);

        // FIXME: WTF was I thinking?
        if ((stream.get_num_remaining_bits() == 2) && ((i == num_ops - 3) || (i == num_ops - 2))) {
            stream.skip(2);
        }

        if (remaining_ops == 0) {
            if (op & 0x20) {
                remaining_ops = (op & 0x1F) + 1;
            } else {
                for (int j = 0; j < (op&0x1F)+1; ++j) {
                    assert(num_pixels < frame_width*frame_height);
                    *pixels++ = palette.at(0);
                    ++num_pixels;
                }
            }
        } else {
            assert(num_pixels < frame_width*frame_height);
            *pixels++ = palette.at(op);
            ++num_pixels;
            remaining_ops -= 1;
        }
    }
    assert(num_pixels == frame_width * frame_height);

    ptr += align_to_n(num_bytes, 4);
    size -= align_to_n(num_bytes, 4);

    return image;
}

void AnimParser::ParseFrameInfos()
{
    while (size >= 8) {
        FrameInfo info;
        info.frame_offset = *(uint32_t*)ptr;
        ptr += 4; size -= 4;
        info.magic = *(uint32_t*)ptr;
        ptr += 4; size -= 4;

        // frame infos always start with a valid frame offset
        if (std::find(frame_offsets.begin(), frame_offsets.end(), info.frame_offset) == frame_offsets.end()) {
            ptr -= 8; size += 8;
            return;
        }

        switch (info.magic) {
            case 0x00000003:
                frame_info_offsets.push_back(ptr - 8 - (const uint8_t*)data);
                ParseFrameInfo00000003(&info);
                break;
            case 0x00000005:
                frame_info_offsets.push_back(ptr - 8 - (const uint8_t*)data);
                ParseFrameInfo00000005(&info);
                break;
            case 0x00010003:
                frame_info_offsets.push_back(ptr - 8 - (const uint8_t*)data);
                ParseFrameInfo00010003(&info);
                break;
            default:
                // not a frame info
                ptr -= 8; size += 8;
                return;
        }

        frame_infos.push_back(info);
    }
}

void AnimParser::ParseFrameInfo00000003(AnimParser::FrameInfo* info)
{
    assert(size >= 8);
    info->x1 = *(int16_t*)ptr;
    ptr += 2; size -= 2;
    info->y1 = *(int16_t*)ptr;
    ptr += 2; size -= 2;
    info->x2 = *(int16_t*)ptr;
    ptr += 2; size -= 2;
    info->y2 = *(int16_t*)ptr;
    ptr += 2; size -= 2;

    assert(size >= 4);
    ptr += 4; size -= 4;
}

void AnimParser::ParseFrameInfo00000005(AnimParser::FrameInfo* info)
{
    assert(size >= 8);
    info->x1 = *(int16_t*)ptr;
    ptr += 2; size -= 2;
    info->y1 = *(int16_t*)ptr;
    ptr += 2; size -= 2;
    info->x2 = *(int16_t*)ptr;
    ptr += 2; size -= 2;
    info->y2 = *(int16_t*)ptr;
    ptr += 2; size -= 2;

    assert(size >= 12);
    ptr += 12; size -= 12;
}

void AnimParser::ParseFrameInfo00010003(AnimParser::FrameInfo* info)
{
    assert(size >= 8);
    info->x1 = *(int16_t*)ptr;
    ptr += 2; size -= 2;
    info->y1 = *(int16_t*)ptr;
    ptr += 2; size -= 2;
    info->x2 = *(int16_t*)ptr;
    ptr += 2; size -= 2;
    info->y2 = *(int16_t*)ptr;
    ptr += 2; size -= 2;

    assert(size >= 12);
    // FIXME: are these hardcoded?
    if (*(ptr+9) == 0x00) {
        // 1xABEBSIC.BAN 2xABETHROW.BAN 1xSLIG.BND 15xSLOG.BND 2xRTHROW.BND 2xMTHROW.BND 2xGTHROW.BND
        assert(size >= 12);
        ptr += 12; size -= 12;
    } else if (*(ptr+9) == 0xFF) {
        // 6xF2ZBALL.BAN
        assert(size >= 16);
        ptr += 16; size -= 16;
    } else {
        assert(!"Bad frame info");
    }
}

void AnimParser::ParseFrameSequences()
{
    while (size >= 8) {
        SequenceInfo sequence;
        sequence.ticks_per_frame = *(uint16_t*)ptr;
        ptr += 2; size -= 2;
        sequence.num_frames = *(uint16_t*)ptr;
        ptr += 2; size -= 2;
        sequence.unknown1 = *(uint16_t*)ptr;
        ptr += 2; size -= 2;
        sequence.unknown2 = *(uint16_t*)ptr;
        ptr += 2; size -= 2;

        assert(size >= sequence.num_frames * 4);
        for (uint16_t i = 0; i < sequence.num_frames; ++i) {
            sequence.frame_info_offsets.push_back(*(uint32_t*)ptr);
            ptr += 4; size -= 4;
        }

        sequence_infos.push_back(sequence);
    }
}

std::unique_ptr<AnimSet> ResAnimLoad(unsigned long id, const void* data, unsigned long size)
{
    std::unique_ptr<AnimSet> anim_set(new AnimSet());
    anim_set->id = id;

    AnimParser parser((const uint8_t*)data, (uint32_t)size);
    anim_set->images = std::move(parser.images);

    for (const AnimParser::SequenceInfo seqinfo : parser.sequence_infos) {
        AnimSequence sequence;
        sequence.ticks_per_frame = seqinfo.ticks_per_frame;

        for (uint32_t frame_info_offset : seqinfo.frame_info_offsets) {
            const AnimParser::FrameInfo* frminfo = &parser.frame_infos.at(parser.FindFrameInfoByOffset(frame_info_offset));
            assert(frminfo != nullptr);

            Image* image = anim_set->images.at(parser.FindFrameByOffset(frminfo->frame_offset)).get();
            assert(image != nullptr);

            AnimFrame frame;
            frame.image = image;
            frame.x_offset = frminfo->x1;
            frame.y_offset = frminfo->y1;
            sequence.frames.push_back(frame);
        }

        anim_set->sequences.push_back(sequence);
    }

    return anim_set;
}
