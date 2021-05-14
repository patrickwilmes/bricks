set(SOURCES
        types.h
        renderer.c
        renderer.h
        event.c
        event.h
        paddle.c
        paddle.h
        ball.c
        ball.h
        brick.c
        brick.h
        main.c
        )

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(libpng CONFIG REQUIRED)

add_executable(Bricks ${SOURCES})
target_link_libraries(Bricks PRIVATE SDL2::SDL2main SDL2::SDL2-static SDL2::SDL2_image SDL2::SDL2_ttf png_static)
