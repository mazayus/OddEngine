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

#ifndef ODD_LEVEL_H
#define ODD_LEVEL_H

#include "Animation.h"
#include "Image.h"
#include "entities/Entity.h"
#include "res/ResPath.h"

#include <memory>
#include <string>
#include <vector>

class Renderer;

class OddResource;
class OddArchive;

class OddCam;
class OddPath;
class OddLevel;

/*
 * OddCam
 */

class OddCam
{
public:
    OddCam(const OddPath* path, const OddArchive* archive, const OddResource* resource);

    unsigned int GetCamId() const;
    const Image* GetBackgroundImage() const;

private:
    OddCam(const OddCam&) = delete;
    OddCam& operator=(const OddCam&) = delete;

    unsigned int cam_id;
    std::unique_ptr<Image> background_image;
};

/*
 * OddPath
 */

class OddPath
{
public:
    OddPath(const OddLevel* level, const OddArchive* archive, const OddResource* resource);

    unsigned int GetPathId() const;

    void ActivateCam(unsigned int id);
    void ActivateLeftCam();
    void ActivateRightCam();
    void ActivateTopCam();
    void ActivateBottomCam();
    OddCam* GetActiveCam();

    const std::vector<std::unique_ptr<OddCam>>& GetCams() const;

    const std::vector<ResPath::CollisionItem>& GetCollisionItems() const;
    const std::vector<std::unique_ptr<Entity>>& GetEntities() const;

    void SetGridSize(int rows, int columns);

    int GetNumGridRows() const;
    int GetNumGridColumns() const;

    int GetCurrentGridRow() const;
    int GetCurrentGridColumn() const;

    void Update();
    void Render(Renderer* renderer);

private:
    OddPath(const OddPath&) = delete;
    OddPath& operator=(const OddPath&) = delete;

    unsigned int path_id;

    int active_cam_index;
    std::vector<std::unique_ptr<OddCam>> cams;

    std::vector<ResPath::CollisionItem> collision_items;
    std::vector<std::unique_ptr<Entity>> entities;

    int num_rows;
    int num_columns;
};

/*
 * OddLevel
 */

class OddLevel
{
public:
    OddLevel(const OddArchive* archive);

    void ActivatePath(unsigned int id);
    void ActivateNextPath();
    void ActivatePreviousPath();
    OddPath* GetActivePath();

    const std::vector<std::unique_ptr<OddPath>>& GetPaths() const;

    const AnimSet* FindAnimSet(unsigned long id) const;
    const std::vector<std::unique_ptr<AnimSet>>& AnimSets() const;

private:
    OddLevel(const OddLevel&) = delete;
    OddLevel& operator=(const OddLevel&) = delete;

    unsigned int active_path_index;
    std::vector<std::unique_ptr<OddPath>> paths;
    std::vector<std::unique_ptr<AnimSet>> anim_sets;
};

#endif
