cmake_minimum_required(VERSION 3.16)
project(game-engine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
find_package(SFML 2 REQUIRED COMPONENTS graphics audio network system window)

add_library(game-engine STATIC
    core/gameEngine.cpp
    core/scroller.cpp
    core/updateThread.cpp
    core/audio/sound.cpp
    core/graphics/animatedGraphic.cpp
    core/graphics/animation.cpp
    core/graphics/drawable.cpp
    core/graphics/image.cpp
    core/graphics/rectangle.cpp
    core/graphics/shape.cpp
    core/graphics/sprite.cpp
    core/input/keyboardHandler.cpp
    dataloader/dataLoader.cpp
    dataloader/factory/platformMultiFactory.cpp
    dataloader/factory/platformSingleFactory.cpp
    dataloader/factory/stageFactory.cpp
    util/timer.cpp
)

target_include_directories(game-engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/core/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/core/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/core/input
    ${CMAKE_CURRENT_SOURCE_DIR}/dataloader
    ${CMAKE_CURRENT_SOURCE_DIR}/dataloader/factory
    ${CMAKE_CURRENT_SOURCE_DIR}/dataloader/model
    ${CMAKE_CURRENT_SOURCE_DIR}/util
)
target_include_directories(game-engine SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/json
)

target_link_libraries(game-engine PUBLIC
    sfml-graphics
    sfml-audio
    sfml-network
    sfml-system
    sfml-window
)
