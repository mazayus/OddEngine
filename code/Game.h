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

#ifndef GAME_H
#define GAME_H

#include <map>
#include <string>

#include "OddArchive.h"
#include "OddLevel.h"

class Game
{
public:
    Game(const std::string& gamepath);
    ~Game();

    std::unique_ptr<OddLevel> LoadLevel(const std::string& id);

    void DBG_LogPathGridSizes();

private:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    struct PathInfo
    {
        int width, height;
    };

    struct LevelInfo
    {
        std::string filename;
        std::map<int, PathInfo> path_infos;
    };

    struct GameInfo
    {
        std::string gamepath;
        std::map<std::string, LevelInfo> level_infos;
    };

    GameInfo game_info;
};

#endif
