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

#ifndef LUA_VALUE_H
#define LUA_VALUE_H

#include <map>
#include <memory>
#include <string>
#include <vector>

class LuaValue
{
public:
    static std::unique_ptr<LuaValue> Parse(const std::string& filename);

public:
    LuaValue() = default;
    virtual ~LuaValue() = default;

private:
    LuaValue(const LuaValue&) = delete;
    LuaValue& operator=(const LuaValue&) = delete;
};

class LuaTable;
class LuaNumber;
class LuaString;

class LuaTable : public LuaValue
{
public:
    LuaTable(struct lua_State* lua);
    ~LuaTable() = default;

    size_t Length();

    const std::vector<int>& IntKeys() const;
    LuaValue& operator[](int key);
    LuaTable& GetTable(int key);
    LuaNumber& GetNumber(int key);
    LuaString& GetString(int key);

    const std::vector<std::string>& StringKeys() const;
    LuaValue& operator[](const std::string& key);
    LuaTable& GetTable(const std::string& key);
    LuaNumber& GetNumber(const std::string& key);
    LuaString& GetString(const std::string& key);

private:
    LuaTable(const LuaTable&) = delete;
    LuaTable& operator=(const LuaTable&) = delete;

    size_t length;

    std::vector<int> int_keys;
    std::map<int, std::unique_ptr<LuaValue>> int_key_map;

    std::vector<std::string> string_keys;
    std::map<std::string, std::unique_ptr<LuaValue>> string_key_map;
};

class LuaNumber : public LuaValue
{
public:
    LuaNumber(struct lua_State* lua);
    ~LuaNumber() = default;

    int ToInt();
    float ToFloat();

private:
    LuaNumber(const LuaNumber&) = delete;
    LuaNumber& operator=(const LuaNumber&) = delete;

    double value;
};

class LuaString : public LuaValue
{
public:
    LuaString(struct lua_State* lua);
    ~LuaString() = default;

    std::string ToString();

private:
    LuaString(const LuaString&) = delete;
    LuaString& operator=(const LuaString&) = delete;

    std::string value;
};

#endif
