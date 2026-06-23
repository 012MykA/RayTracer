cmake_minimum_required(VERSION 4.3)
project(RayTracer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE RAY_TRACER_SOURCES
    src/*.cpp
)
add_executable(${PROJECT_NAME} ${RAY_TRACER_SOURCES})

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${Stb_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        glm::glm
)
