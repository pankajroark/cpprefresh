cmake_minimum_required(VERSION 3.16)

project(mt)

# Protobuf
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")
find_program(_PROTOBUF_PROTOC protoc)
message(STATUS "Using protoc ${_PROTOBUF_PROTOC}")


get_filename_component(config_proto "protos/config.proto" ABSOLUTE)
get_filename_component(config_proto_path "${config_proto}" PATH)


set(config_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/config.pb.cc")
set(config_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/config.pb.h")


add_custom_command(
      OUTPUT "${config_proto_srcs}" "${config_proto_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${config_proto_path}"
        "${config_proto}"
      DEPENDS "${config_proto}"
      COMMENT "Running C++ protocol buffer compiler on ${config_proto}"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Protobuf end -- kind of, still need to include proto src and target

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/cxxopts)

# add_executable(mt src/main.cpp)
# add_executable(mt src/worker_example.cpp)
# add_executable(mt src/cv_example.cpp)
# add_executable(mt src/cxxopts_ex.cpp)
add_executable(
  mt 
  src/pbtxt_ex.cpp
  ${config_proto_srcs}
)

target_link_libraries(
  mt 
  pthread 
  cxxopts
  protobuf::libprotobuf
)
