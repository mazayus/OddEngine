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

#ifndef ENTITY_H
#define ENTITY_H

class OddLevel;
class Renderer;

class Entity
{
public:
    enum Type
    {
        Type_Pullring = 18,
        Type_BackgroundAnim = 19,
        Type_TimeBomb = 22,
        Type_Slig = 24,
        Type_Slog = 25,
        Type_Lever = 26,
        Type_Mamorb = 29,
        Type_Mine = 46,
        Type_Bomb = 47,
        Type_Trapdoor = 55,
        Type_Eyeorb = 61,
        Type_MotionDetector = 62,
        Type_ElectricWall = 67,
        Type_Scrab = 72,
        Type_Mudokon = 82,
        Type_MeatSaw = 88,
        Type_BoomMachine = 97,

        Type_Unknown = 255
    };

public:
    static Entity* Create(const OddLevel* level, Type type, const void* data, unsigned int size);

public:
    Entity(Type type, const void* data, unsigned int size);
    virtual ~Entity() = default;

    Type GetType();

    virtual void Think();
    virtual void Render(Renderer* renderer);

    int x, y;
    int x1, y1;
    int x2, y2;

private:
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    Type type;
};

#endif
