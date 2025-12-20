cmake_minimum_required(VERSION 3.20)
project(P03_GRPC)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(protolib proto/a.proto)

target_link_libraries(protolib gRPC::grpc++ protobuf::libprotobuf)

target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

protobuf_generate(TARGET protolib LANGUAGE cpp)

protobuf_generate(TARGET protolib LANGUAGE grpc  GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc  PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

add_executable(P03_GRPC src/server.cpp)
target_link_libraries(P03_GRPC protolib)
