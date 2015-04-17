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

#include "Game.h"

#include <stdio.h>

#include "LuaValue.h"

Game::Game(const std::string& gamepath)
{
    std::unique_ptr<LuaValue> lua_value = LuaValue::Parse(gamepath + "/_game.lua");
    LuaTable& game_table = dynamic_cast<LuaTable&>(*lua_value.get());
    game_info.gamepath = gamepath;

    LuaTable& all_levels_table = game_table.GetTable("levels");
    for (int i : all_levels_table.IntKeys()) {
        LevelInfo level_info;
        LuaTable& level_table = all_levels_table.GetTable(i);
        std::string level_id = level_table.GetString("id").ToString();
        level_info.filename = level_table.GetString("filename").ToString();

        LuaTable& all_paths_table = level_table.GetTable("paths");
        for (int j : all_paths_table.IntKeys()) {
            PathInfo path_info;
            LuaTable& path_table = all_paths_table.GetTable(j);
            int path_id = path_table.GetNumber("id").ToInt();
            path_info.width = path_table.GetNumber("width").ToInt();
            path_info.height = path_table.GetNumber("height").ToInt();
            level_info.path_infos[path_id] = path_info;
        }

        game_info.level_infos[level_id] = level_info;
    }
}

Game::~Game()
{

}

std::unique_ptr<OddLevel> Game::LoadLevel(const std::string& id)
{
    auto level_info_it = game_info.level_infos.find(id);
    if (level_info_it == game_info.level_infos.end())
        throw std::runtime_error("Game::LoadLevel(): invalid level id");

    const LevelInfo& level_info = level_info_it->second;
    std::string fullpath = game_info.gamepath + "/" + level_info.filename;
    OddArchive archive(fullpath.c_str());

    std::unique_ptr<OddLevel> level(new OddLevel(&archive));
    for (auto path_info_it : level_info.path_infos) {
        int path_id = path_info_it.first;
        const PathInfo& path_info = path_info_it.second;
        level->ActivatePath(path_id);
        OddPath* path = level->GetActivePath();
        path->SetGridSize(path_info.height, path_info.width);
    }

    level->ActivatePath(level_info.path_infos.begin()->first);
    return level;
}

void Game::DBG_LogPathGridSizes()
{
    for (auto level_it : game_info.level_infos) {
        printf("LEVEL: id = %s, filename = %s\n",
               level_it.first.c_str(), level_it.second.filename.c_str());
        for (auto path_it : level_it.second.path_infos) {
            printf("  PATH: id = %d, width = %d, height = %d\n",
                   path_it.first, path_it.second.width, path_it.second.height);
        }
    }
}
