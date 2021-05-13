set(SOURCES
        Application.cpp
        Application.h
        main.cpp
        )

add_executable(Bricks ${SOURCES})
target_link_libraries(Bricks LibGraphics)