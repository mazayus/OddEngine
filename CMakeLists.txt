cmake_minimum_required(VERSION 3.0)

project(OddEngine)

add_executable(OddEngine
    code/main.cpp
    code/utils.cpp
    code/Animation.cpp
    code/Game.cpp
    code/Image.cpp
    code/LuaValue.cpp
    code/OddArchive.cpp
    code/OddLevel.cpp
    code/Renderer.cpp
    code/entities/Entity.cpp
    code/entities/Entity_BackgroundAnim.cpp
    code/entities/Entity_Bomb.cpp
    code/entities/Entity_BoomMachine.cpp
    code/entities/Entity_ElectricWall.cpp
    code/entities/Entity_Eyeorb.cpp
    code/entities/Entity_Lever.cpp
    code/entities/Entity_Mamorb.cpp
    code/entities/Entity_MeatSaw.cpp
    code/entities/Entity_Mine.cpp
    code/entities/Entity_MotionDetector.cpp
    code/entities/Entity_Mudokon.cpp
    code/entities/Entity_Pullring.cpp
    code/entities/Entity_Scrab.cpp
    code/entities/Entity_Slig.cpp
    code/entities/Entity_Slog.cpp
    code/entities/Entity_TimeBomb.cpp
    code/entities/Entity_Trapdoor.cpp
    code/res/ResAnim.cpp
    code/res/ResBits.cpp
    code/res/ResFG1.cpp
    code/res/ResFont.cpp
    code/res/ResIndx.cpp
    code/res/ResPalt.cpp
    code/res/ResPath.cpp
)

target_compile_options(OddEngine PUBLIC
    -std=c++11 -pedantic -Wall -Wextra
    -fno-strict-aliasing
)

target_link_libraries(OddEngine
    SDL2
    lua5.2
)
