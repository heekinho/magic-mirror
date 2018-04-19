project(magic-mirror)
cmake_minimum_required(VERSION 3.2)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#add_compile_options(-std=c++11)

# GRPC and Protocol Buffers libraries location
list(APPEND CMAKE_PREFIX_PATH "/opt/grpc" "/opt/protobuf")

# Cmake find modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

#find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
#include_directories(${OpenCV_INCLUDE_DIRS})

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")


set(PROTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/protos/magicmirror.proto
)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-src)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})

# Building server
add_executable(magic_mirror_server
    ${CMAKE_CURRENT_SOURCE_DIR}/magic_mirror_server.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/image_operations.cc
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)
target_link_libraries(magic_mirror_server
    gRPC::grpc++_reflection
    protobuf::libprotobuf
)

# Building client
add_executable(magic_mirror_client
    ${CMAKE_CURRENT_SOURCE_DIR}/magic_mirror_client.cc
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)
target_link_libraries(magic_mirror_client
    ${OpenCV_LIBS}
    gRPC::grpc++_reflection
    protobuf::libprotobuf
)
