cmake_minimum_required(VERSION 3.16)
project(ghosty CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../game-engine ${CMAKE_BINARY_DIR}/game-engine)

add_executable(ghosty
    main.cpp
    ghosty.cpp
    ghostyStageFactory.cpp
    movingPlatform.cpp
    raindrop.cpp
)

target_include_directories(ghosty PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ghosty PRIVATE game-engine)

file(GLOB_RECURSE GHOSTY_IMAGES "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/*.png"
                                "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/*.jpg"
                                "${CMAKE_CURRENT_SOURCE_DIR}/assets/images/*.jpeg")
file(GLOB_RECURSE GHOSTY_SOUNDS "${CMAKE_CURRENT_SOURCE_DIR}/assets/sound/*.wav"
                                "${CMAKE_CURRENT_SOURCE_DIR}/assets/sound/*.ogg")
set(ENGINE_FONTS "${CMAKE_CURRENT_SOURCE_DIR}/../game-engine/assets/arial.ttf")

foreach(_asset ${GHOSTY_IMAGES} ${GHOSTY_SOUNDS} ${ENGINE_FONTS})
    get_filename_component(_fname "${_asset}" NAME)
    add_custom_command(TARGET ghosty POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_asset}" "$<TARGET_FILE_DIR:ghosty>/${_fname}"
        COMMENT "Copying asset ${_fname}")
endforeach()
