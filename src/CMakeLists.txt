# CMakeList.txt : CMake project for raycode, include source and define
# project specific logic here.
#

# Set raylib path
set(RAYLIB_ROOT "C:/raylib/raylib")
set(RAYLIB_SRC "${RAYLIB_ROOT}/src")

# Add Box2D library
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/box2d)

# Include raylib headers and external dependencies
include_directories(${RAYLIB_SRC})
include_directories(${RAYLIB_SRC}/external)
include_directories(${RAYLIB_SRC}/external/glfw/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/box2d/include)

# Collect raylib source files for static linking
file(GLOB RAYLIB_SOURCES 
    "${RAYLIB_SRC}/rcore.c"
    "${RAYLIB_SRC}/rshapes.c"
    "${RAYLIB_SRC}/rtextures.c"
    "${RAYLIB_SRC}/rtext.c"
    "${RAYLIB_SRC}/rmodels.c"
    "${RAYLIB_SRC}/utils.c"
    "${RAYLIB_SRC}/raudio.c"
)

# Add platform-specific files
if(WIN32)
    list(APPEND RAYLIB_SOURCES "${RAYLIB_SRC}/rglfw.c")
endif()

# Add source to this project's executable.
add_executable (raycode 
    "raycode.cpp" 
    "raycode.h" 
    "game.h" 
    "game.cpp" 
    "ball.h" 
    "ball.cpp"
    "brick.h"
    "brick.cpp"
    "wall.h"
    "wall.cpp"
    ${RAYLIB_SOURCES}
)

# Set C++ standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET raycode PROPERTY CXX_STANDARD 20)
endif()

# Link Box2D
target_link_libraries(raycode PRIVATE box2d)

# Platform-specific definitions and libraries
if(WIN32)
    target_compile_definitions(raycode PRIVATE PLATFORM_DESKTOP)
    target_link_libraries(raycode PRIVATE 
        winmm 
        gdi32 
        opengl32
    )
endif()

# Compiler-specific settings for raylib
if(MSVC)
    target_compile_options(raycode PRIVATE /W3)
    # Disable specific warnings for raylib
    target_compile_options(raycode PRIVATE /wd4996 /wd4244 /wd4267)
endif()
