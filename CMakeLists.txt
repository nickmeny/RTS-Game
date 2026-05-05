cmake_minimum_required(VERSION 3.11)

# Προσθήκη για να φύγει το Warning με το DOWNLOAD_EXTRACT_TIMESTAMP
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(MyRaylibGame LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

# ΔΙΟΡΘΩΣΗ: Εδώ γράψε το σωστό path για το αρχείο σου
# Αν το main.c είναι μέσα στο src/main.c:
add_executable(${PROJECT_NAME} src/game.c) 

target_link_libraries(${PROJECT_NAME} raylib)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m pthread dl rt X11)
endif()
