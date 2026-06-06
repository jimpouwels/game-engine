cmake_minimum_required(VERSION 3.16)
project(GameEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# SFML (Homebrew arm64 install — sfml@2)
# ---------------------------------------------------------------------------
set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")
find_package(SFML 2 REQUIRED COMPONENTS graphics audio network system window)

# ---------------------------------------------------------------------------
# game-engine (static library)
# ---------------------------------------------------------------------------
add_library(game-engine STATIC
    game-engine/core/gameEngine.cpp
    game-engine/core/scroller.cpp
    game-engine/core/updateThread.cpp
    game-engine/core/audio/sound.cpp
    game-engine/core/graphics/animatedGraphic.cpp
    game-engine/core/graphics/animation.cpp
    game-engine/core/graphics/drawable.cpp
    game-engine/core/graphics/image.cpp
    game-engine/core/graphics/rectangle.cpp
    game-engine/core/graphics/shape.cpp
    game-engine/core/graphics/sprite.cpp
    game-engine/core/input/keyboardHandler.cpp
    game-engine/dataloader/dataLoader.cpp
    game-engine/dataloader/factory/platformMultiFactory.cpp
    game-engine/dataloader/factory/platformSingleFactory.cpp
    game-engine/dataloader/factory/stageFactory.cpp
    game-engine/util/timer.cpp
)

target_include_directories(game-engine PUBLIC
    game-engine/core
    game-engine/core/graphics
    game-engine/core/audio
    game-engine/core/input
    game-engine/dataloader
    game-engine/dataloader/factory
    game-engine/dataloader/model
    game-engine/util
)
target_include_directories(game-engine SYSTEM PUBLIC
    game-engine/json
)

target_link_libraries(game-engine PUBLIC
    sfml-graphics
    sfml-audio
    sfml-network
    sfml-system
    sfml-window
)

# ---------------------------------------------------------------------------
# ghosty (executable)
# ---------------------------------------------------------------------------
add_executable(ghosty
    ghosty/game.cpp
    ghosty/ghosty.cpp
    ghosty/ghostyStageFactory.cpp
    ghosty/movingPlatform.cpp
    ghosty/raindrop.cpp
)

target_include_directories(ghosty PRIVATE ghosty)
target_link_libraries(ghosty PRIVATE game-engine)

# ---------------------------------------------------------------------------
# Copy ghosty assets (images + sounds) flat into the build directory so the
# game finds them by bare filename at runtime.
# ---------------------------------------------------------------------------
file(GLOB_RECURSE GHOSTY_IMAGES "${CMAKE_SOURCE_DIR}/ghosty/assets/images/*.png"
                                "${CMAKE_SOURCE_DIR}/ghosty/assets/images/*.jpg"
                                "${CMAKE_SOURCE_DIR}/ghosty/assets/images/*.jpeg")
file(GLOB_RECURSE GHOSTY_SOUNDS "${CMAKE_SOURCE_DIR}/ghosty/assets/sound/*.wav"
                                "${CMAKE_SOURCE_DIR}/ghosty/assets/sound/*.ogg")
set(ENGINE_FONTS "${CMAKE_SOURCE_DIR}/game-engine/assets/arial.ttf")

foreach(_asset ${GHOSTY_IMAGES} ${GHOSTY_SOUNDS} ${ENGINE_FONTS})
    get_filename_component(_fname "${_asset}" NAME)
    add_custom_command(TARGET ghosty POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_asset}" "$<TARGET_FILE_DIR:ghosty>/${_fname}"
        COMMENT "Copying asset ${_fname}")
endforeach()
