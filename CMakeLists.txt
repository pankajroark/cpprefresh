cmake_minimum_required(VERSION 3.16)

project(mt)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/cxxopts)

# add_executable(mt src/main.cpp)
# add_executable(mt src/worker_example.cpp)
# add_executable(mt src/cv_example.cpp)
add_executable(mt src/cxxopts_ex.cpp)

target_link_libraries(mt pthread cxxopts)
