cmake_minimum_required(VERSION 3.16)

project(mt)

# make sure clang-tidy is installed, apt install -y clang-tidy
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*,-modernize-use-trailing-return-type,-modernize-use-nodiscard,-modernize-use-override,-modernize-use-equals-default,-modernize-use-equals-delete,-modernize-use-nullptr,-modernize-use-bool-literals,-modernize-use-auto,-modernize-pass-by-value,-modernize-raw-string-literal")



message(STATUS "Using clang-tidy: ${CLANG_TIDY_EXECUTABLE}")
message(STATUS "Using clang-tidy: ${CLANG_TIDY_COMMAND}")

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

# grpc
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
message(STATUS "Using _GRPC_CPP_PLUGIN_EXECUTABLE ${_GRPC_CPP_PLUGIN_EXECUTABLE}")


get_filename_component(route_guide_proto "protos/route_guide.proto" ABSOLUTE)
get_filename_component(route_guide_proto_path "${route_guide_proto}" PATH)

set(route_guide_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/route_guide.pb.cc")
set(route_guide_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/route_guide.pb.h")
set(route_guide_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/route_guide.grpc.pb.cc")
set(route_guide_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/route_guide.grpc.pb.h")

add_custom_command(
      OUTPUT "${route_guide_proto_srcs}" "${route_guide_proto_hdrs}" "${route_guide_grpc_srcs}" "${route_guide_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${route_guide_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${route_guide_proto}"
      DEPENDS "${route_guide_proto}"
      COMMENT "Running C++ protocol buffer compiler on ${route_guide_proto}"
)


include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Protobuf end -- kind of, still need to include proto src and target

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/cxxopts)

# add_executable(mt src/main.cpp)
# add_executable(mt src/worker_example.cpp)
# add_executable(mt src/cv_example.cpp)
# add_executable(mt src/map_ex.cpp)
# add_executable(mt src/cxxopts_ex.cpp)
# add_executable(
#   mt 
#   src/pbtxt_ex.cpp
#   ${config_proto_srcs}
# )
# add_executable(
#   mt 
#   src/grpc_streaming_server.cpp
#   ${route_guide_proto_srcs}
#   ${route_guide_grpc_srcs}
# )

add_executable(
  mt_client 
  src/grpc_streaming_client.cpp
  ${route_guide_proto_srcs}
  ${route_guide_grpc_srcs}
)

add_executable(
  mt
  src/grpc_streaming_cb_server.cpp
  ${route_guide_proto_srcs}
  ${route_guide_grpc_srcs}
)

# clang-tidy
# set_target_properties(mt PROPERTIES CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

target_link_libraries(
  mt 
  pthread 
  cxxopts
  protobuf::libprotobuf
  gRPC::grpc++_reflection
)

target_link_libraries(
  mt_client
  pthread 
  cxxopts
  protobuf::libprotobuf
  gRPC::grpc++_reflection
)