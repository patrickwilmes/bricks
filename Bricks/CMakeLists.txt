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
        level.c
        level.h
        main.c
        )

add_executable(Bricks ${SOURCES})
target_link_libraries(Bricks PRIVATE ${CONAN_LIBS})
