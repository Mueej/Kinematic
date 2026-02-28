cmake_minimum_required(VERSION 3.10)
project(kinematics2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

include_directories(include)

find_package(OpenGL REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(app ${SOURCES})

target_link_libraries(app PRIVATE OpenGL::GL glfw)

if(APPLE)
    target_link_libraries(app PRIVATE "-framework Cocoa" "-framework IOKit")
endif()

