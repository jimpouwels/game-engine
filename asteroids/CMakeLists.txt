cmake_minimum_required(VERSION 3.16)
project(asteroids CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../game-engine ${CMAKE_BINARY_DIR}/game-engine)

add_executable(asteroids
    main.cpp
    asteroid.cpp
    asteroidSpawner.cpp
    bullet.cpp
    ship.cpp
)

target_include_directories(asteroids PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(asteroids PRIVATE game-engine)

file(GLOB ASTEROIDS_ASSETS "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
set(ENGINE_FONTS "${CMAKE_CURRENT_SOURCE_DIR}/../game-engine/assets/arial.ttf")

foreach(_asset ${ASTEROIDS_ASSETS} ${ENGINE_FONTS})
    get_filename_component(_fname "${_asset}" NAME)
    add_custom_command(TARGET asteroids POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_asset}" "$<TARGET_FILE_DIR:asteroids>/${_fname}"
        COMMENT "Copying asset ${_fname}")
endforeach()
