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

#include "LuaValue.h"

extern "C" {
#include <lua5.2/lua.h>
#include <lua5.2/lualib.h>
#include <lua5.2/lauxlib.h>
}

#include <stdexcept>

std::unique_ptr<LuaValue> LuaValue::Parse(const std::string& filename)
{
    std::unique_ptr<lua_State, void(*)(lua_State*)> lua(luaL_newstate(), lua_close);

    if (!lua)
        throw std::runtime_error("LuaValue: can't create lua state object");
    if (luaL_dofile(lua.get(), filename.c_str()) != 0)
        throw std::runtime_error("LuaValue: can't execute lua script");
    if (lua_gettop(lua.get()) == 0)
        throw std::runtime_error("LuaValue: lua script didn't return a value");

    switch (lua_type(lua.get(), -1)) {
        case LUA_TTABLE:
            return std::unique_ptr<LuaValue>(new LuaTable(lua.get()));
            break;
        case LUA_TNUMBER:
            return std::unique_ptr<LuaValue>(new LuaNumber(lua.get()));
            break;
        case LUA_TSTRING:
            return std::unique_ptr<LuaValue>(new LuaString(lua.get()));
            break;
        default:
            throw std::runtime_error("LuaValue: bad value type");
    }
}

LuaTable::LuaTable(lua_State* lua)
{
    if (lua_type(lua, -1) != LUA_TTABLE)
        throw std::runtime_error("LuaTable: bad value type");

    length = lua_rawlen(lua, -1);

    lua_pushnil(lua);
    while (lua_next(lua, -2)) {
        std::unique_ptr<LuaValue> value;
        // value
        switch (lua_type(lua, -1)) {
            case LUA_TTABLE:
                value.reset(new LuaTable(lua));
                break;
            case LUA_TNUMBER:
                value.reset(new LuaNumber(lua));
                break;
            case LUA_TSTRING:
                value.reset(new LuaString(lua));
                break;
            default:
                throw std::runtime_error("LuaTable: bad value type");
        }
        // key
        switch (lua_type(lua, -2)) {
            case LUA_TNUMBER:
                int_keys.push_back(lua_tointeger(lua, -2));
                int_key_map[int_keys.back()] = std::move(value);
                break;
            case LUA_TSTRING:
                string_keys.push_back(lua_tostring(lua, -2));
                string_key_map[string_keys.back()] = std::move(value);
                break;
            default:
                throw std::runtime_error("LuaTable: bad key type");
        }
        lua_pop(lua, 1);
    }
}

size_t LuaTable::Length()
{
    return length;
}

const std::vector<int>& LuaTable::IntKeys() const
{
    return int_keys;
}

LuaValue& LuaTable::operator[](int key)
{
    auto it = int_key_map.find(key);
    if (it == int_key_map.end())
        throw std::runtime_error("LuaTable: key isn't in the table");
    return *it->second;
}

LuaTable& LuaTable::GetTable(int key)
{
    return dynamic_cast<LuaTable&>(operator[](key));
}

LuaNumber& LuaTable::GetNumber(int key)
{
    return dynamic_cast<LuaNumber&>(operator[](key));
}

LuaString& LuaTable::GetString(int key)
{
    return dynamic_cast<LuaString&>(operator[](key));
}

const std::vector<std::string>& LuaTable::StringKeys() const
{
    return string_keys;
}

LuaValue& LuaTable::operator[](const std::string& key)
{
    auto it = string_key_map.find(key);
    if (it == string_key_map.end())
        throw std::runtime_error("LuaTable: key isn't in the table");
    return *it->second;
}

LuaTable& LuaTable::GetTable(const std::string& key)
{
    return dynamic_cast<LuaTable&>(operator[](key));
}

LuaNumber& LuaTable::GetNumber(const std::string& key)
{
    return dynamic_cast<LuaNumber&>(operator[](key));
}

LuaString& LuaTable::GetString(const std::string& key)
{
    return dynamic_cast<LuaString&>(operator[](key));
}

LuaNumber::LuaNumber(lua_State* lua)
{
    if (lua_type(lua, -1) != LUA_TNUMBER)
        throw std::runtime_error("LuaNumber: bad value type");

    value = lua_tonumber(lua, -1);
}

int LuaNumber::ToInt()
{
    return value;
}

float LuaNumber::ToFloat()
{
    return value;
}

LuaString::LuaString(lua_State* lua)
{
    if (lua_type(lua, -1) != LUA_TSTRING)
        throw std::runtime_error("LuaString: bad value type");

    value = lua_tostring(lua, -1);
}

std::string LuaString::ToString()
{
    return value;
}
