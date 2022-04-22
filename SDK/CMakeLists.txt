cmake_minimum_required(VERSION 3.11)

project(SDK)

include(cmake/common.cmake)

get_filename_component(hw_proto "${CMAKE_CURRENT_SOURCE_DIR}/investAPI/src/docs/contracts/*.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

file(GLOB tinkoff_proto_files "${hw_proto_path}/*.proto")

string(REPLACE ${hw_proto_path} ${CMAKE_CURRENT_BINARY_DIR} OUTPUT_FILE_NAMES "${tinkoff_proto_files}")

string(REPLACE ".proto" ".pb.cc" OUTPUT_PB_SOURCES "${OUTPUT_FILE_NAMES}")
string(REPLACE ".proto" ".grpc.pb.cc" OUTPUT_GRPC_SOURCES "${OUTPUT_FILE_NAMES}")
string(REPLACE ".proto" ".pb.h" OUTPUT_PB_HEADERS "${OUTPUT_FILE_NAMES}")
string(REPLACE ".proto" ".grpc.pb.h" OUTPUT_GRPC_HEADERS "${OUTPUT_FILE_NAMES}")

add_custom_command(
      OUTPUT ${OUTPUT_PB_SOURCES} ${OUTPUT_PB_HEADERS} ${OUTPUT_GRPC_SOURCES} ${OUTPUT_GRPC_HEADERS}
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --proto_path ${CMAKE_BINARY_DIR}/_deps/grpc-src/third_party/protobuf/src
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

execute_process(
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fix_keywords.sh ${CMAKE_CURRENT_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RESULT_VARIABLE result
    )

#execute_process(
#    COMMAND "sed -i -- 's/namespace public {/namespace public1 {/g' ${CMAKE_BINARY_DIR}/src/plugins/tinkoffInvest/SDK/*.pb.h ${CMAKE_BINARY_DIR}/src/plugins/tinkoffInvest/SDK/*.pb.cc
#    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/plugins/tinkoffInvest/SDK
#    RESULT_VARIABLE result
#    )

#execute_process(
#    COMMAND "sed -i -- 's/tinkoff::public::/tinkoff::public1::/g' ${CMAKE_BINARY_DIR}/src/plugins/tinkoffInvest/SDK/*.pb.h ${CMAKE_BINARY_DIR}/src/plugins/tinkoffInvest/SDK/*.pb.cc"
#    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/plugins/tinkoffInvest/SDK
#    RESULT_VARIABLE result
#    )

if(result EQUAL 0)
    message(STATUS "public keyword has been changed to public1 in all autogen source files")
endif()

# Include generated *.pb.h files
#include_directories(${CMAKE_BINARY_DIR}/src/plugins/tinkoffInvest/SDK)

# hw_grpc_proto
add_library(hw_grpc_proto
  ${OUTPUT_PB_SOURCES}
  ${OUTPUT_PB_HEADERS}
  ${OUTPUT_GRPC_SOURCES}
  ${OUTPUT_GRPC_HEADERS}
  )
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

find_package(Qt5 REQUIRED COMPONENTS Core Sql)

add_library(${PROJECT_NAME} STATIC
    investapiclient.h
    investapiclient.cpp
    customservice.h
    customservice.cpp
    sandboxservice.h
    sandboxservice.cpp
)

generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Core
    hevaa::interfaces
    hevaa::core
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    )

