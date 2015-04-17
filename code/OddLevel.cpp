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

#include "OddLevel.h"

#include "OddArchive.h"
#include "res/ResAnim.h"
#include "res/ResBits.h"
#include "res/ResPath.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

OddCam::OddCam(const OddPath* path, const OddArchive* archive, const OddResource* resource) :
    cam_id(resource->GetId())
{
    assert(resource->GetType() == OddResource::Type_Bits);
    ResBits res_bits(resource->GetData(), resource->GetSize());

    const Image* res_bits_image = res_bits.GetImage();
    background_image.reset(new Image(res_bits_image->GetWidth(), res_bits_image->GetHeight()));
    memcpy(background_image.get()->GetData(), res_bits_image->GetData(),
            res_bits_image->GetWidth() * res_bits_image->GetHeight() * 4);
}

unsigned int OddCam::GetCamId() const
{
    return cam_id;
}

const Image* OddCam::GetBackgroundImage() const
{
    return background_image.get();
}

OddPath::OddPath(const OddLevel* level, const OddArchive* archive, const OddResource* resource) :
    path_id(resource->GetId()), active_cam_index(0)
{
    assert(resource->GetType() == OddResource::Type_Path);
    ResPath res_path(resource->GetData(), resource->GetSize());

    for (const std::string& cam_name : res_path.GetCameraNames()) {
        if (cam_name.empty()) {
            cams.push_back(nullptr);
            continue;
        }
        const OddFile* file = archive->GetFileByName(cam_name + ".CAM");
        if (!file) {
            cams.push_back(nullptr);
            continue;
        }
        for (unsigned int i = 0; i < file->GetNumResources(); ++i) {
            const OddResource* resource = file->GetResource(i);
            if (resource->GetType() == OddResource::Type_Bits) {
                cams.push_back(std::unique_ptr<OddCam>(new OddCam(this, archive, resource)));
                break;
            }
        }
    }

    collision_items = res_path.GetCollisionItems();

    for (const ResPath::GameObject& go : res_path.GetGameObjects()) {
        Entity* entity = Entity::Create(level, (Entity::Type)go.type, go.data.data(), go.data.size());
        entities.push_back(std::unique_ptr<Entity>(entity));
    }

    num_rows = 1;
    num_columns = res_path.GetCameraNames().size();
}

unsigned int OddPath::GetPathId() const
{
    return path_id;
}

void OddPath::ActivateCam(unsigned int id)
{
    for (unsigned int i = 0; i < cams.size(); ++i) {
        if (cams[i]->GetCamId() == id) {
            active_cam_index = i;
            break;
        }
    }
}

void OddPath::ActivateLeftCam()
{
    int x = active_cam_index % num_columns;
    int y = active_cam_index / num_columns;
    if (x > 0) {
        --x;
        active_cam_index = y * num_columns + x;
    }
}

void OddPath::ActivateRightCam()
{
    int x = active_cam_index % num_columns;
    int y = active_cam_index / num_columns;
    if (x < num_columns - 1) {
        ++x;
        active_cam_index = y * num_columns + x;
    }
}

void OddPath::ActivateTopCam()
{
    int x = active_cam_index % num_columns;
    int y = active_cam_index / num_columns;
    if (y > 0) {
        --y;
        active_cam_index = y * num_columns + x;
    }
}

void OddPath::ActivateBottomCam()
{
    int x = active_cam_index % num_columns;
    int y = active_cam_index / num_columns;
    if (y < num_rows - 1) {
        ++y;
        active_cam_index = y * num_columns + x;
    }
}

OddCam* OddPath::GetActiveCam()
{
    return cams.at(active_cam_index).get();
}

const std::vector<std::unique_ptr<OddCam>>& OddPath::GetCams() const
{
    return cams;
}

const std::vector<ResPath::CollisionItem>& OddPath::GetCollisionItems() const
{
    return collision_items;
}

const std::vector<std::unique_ptr<Entity>>& OddPath::GetEntities() const
{
    return entities;
}

void OddPath::SetGridSize(int rows, int columns)
{
    assert(rows > 0 && columns > 0 && rows * columns == cams.size());
    num_rows = rows;
    num_columns = columns;
}

int OddPath::GetNumGridRows() const
{
    return num_rows;
}

int OddPath::GetNumGridColumns() const
{
    return num_columns;
}

int OddPath::GetCurrentGridRow() const
{
    return active_cam_index / num_columns;
}

int OddPath::GetCurrentGridColumn() const
{
    return active_cam_index % num_columns;
}

void OddPath::Update()
{
    for (std::unique_ptr<Entity>& entity : entities)
        entity->Think();
}

void OddPath::Render(Renderer* renderer)
{
    for (std::unique_ptr<Entity>& entity : entities)
        entity->Render(renderer);
}

OddLevel::OddLevel(const OddArchive* archive) :
    active_path_index(0)
{
    for (unsigned int i = 0; i < archive->GetNumFiles(); ++i) {
        const OddFile* file = archive->GetFile(i);
        for (unsigned int j = 0; j < file->GetNumResources(); ++j) {
            const OddResource* resource = file->GetResource(j);
            if (resource->GetType() == OddResource::Type_Anim) {
                anim_sets.push_back(ResAnimLoad(resource->GetId(), resource->GetData(), resource->GetSize()));
            }
        }
    }

    for (unsigned int i = 0; i < archive->GetNumFiles(); ++i) {
        const OddFile* file = archive->GetFile(i);
        for (unsigned int j = 0; j < file->GetNumResources(); ++j) {
            const OddResource* resource = file->GetResource(j);
            if (resource->GetType() == OddResource::Type_Path) {
                paths.push_back(std::unique_ptr<OddPath>(new OddPath(this, archive, resource)));
            }
        }
    }
}

void OddLevel::ActivatePath(unsigned int id)
{
    for (unsigned int i = 0; i < paths.size(); ++i) {
        if (paths[i]->GetPathId() == id) {
            active_path_index = i;
            break;
        }
    }
}

void OddLevel::ActivateNextPath()
{
    if (active_path_index < paths.size() - 1)
        ++active_path_index;
}

void OddLevel::ActivatePreviousPath()
{
    if (active_path_index > 0)
        --active_path_index;
}

OddPath* OddLevel::GetActivePath()
{
    return paths.at(active_path_index).get();
}

const std::vector<std::unique_ptr<OddPath>>& OddLevel::GetPaths() const
{
    return paths;
}

const AnimSet* OddLevel::FindAnimSet(unsigned long id) const
{
    // TODO: use hash table to speed up search?
    for (const std::unique_ptr<AnimSet>& anim_set : anim_sets)
        if (anim_set->id == id)
            return anim_set.get();
    return nullptr;
}

const std::vector<std::unique_ptr<AnimSet>>& OddLevel::AnimSets() const
{
    return anim_sets;
}
