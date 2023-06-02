cmake_minimum_required(VERSION 3.5.1)

project(TinkoffInvestSDK)

include(GenerateExportHeader)
include(cmake/common.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

file(GLOB tink_protos "${CMAKE_CURRENT_SOURCE_DIR}/investAPI/src/docs/contracts/*.proto")

foreach (tink_proto ${tink_protos})

    get_filename_component(tink_proto_path "${tink_proto}" PATH)
    get_filename_component(tink_proto_name "${tink_proto}" NAME)

    file(GLOB tinkoff_proto_files "${tink_proto}")

    string(REPLACE ${tink_proto_path} ${CMAKE_CURRENT_BINARY_DIR} OUTPUT_FILE_NAMES "${tinkoff_proto_files}")

    string(REPLACE ".proto" "" OUTPUT_NAME "${tink_proto_name}")

    string(REPLACE ".proto" ".pb.cc" OUTPUT_PB_SOURCE "${OUTPUT_FILE_NAMES}")
    string(REPLACE ".proto" ".grpc.pb.cc" OUTPUT_GRPC_SOURCE "${OUTPUT_FILE_NAMES}")
    string(REPLACE ".proto" ".pb.h" OUTPUT_PB_HEADER "${OUTPUT_FILE_NAMES}")
    string(REPLACE ".proto" ".grpc.pb.h" OUTPUT_GRPC_HEADER "${OUTPUT_FILE_NAMES}")

    add_custom_command(
          OUTPUT ${OUTPUT_PB_SOURCE} ${OUTPUT_PB_HEADER} ${OUTPUT_GRPC_SOURCE} ${OUTPUT_GRPC_HEADER}
          COMMAND ${_PROTOBUF_PROTOC}
          ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
            --proto_path ${CMAKE_BINARY_DIR}/_deps/grpc-src/third_party/protobuf/src
            --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
            -I "${tink_proto_path}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${tink_proto}"
          DEPENDS "${tink_proto}")

    file(READ ${OUTPUT_PB_SOURCE} FILE_CONTENTS)
    string(REPLACE "namespace public {" "namespace p_ublic_ {" FILE_CONTENTS "${FILE_CONTENTS}")
    string(REPLACE "tinkoff::public::" "tinkoff::p_ublic_::" FILE_CONTENTS "${FILE_CONTENTS}")
    file(WRITE ${OUTPUT_PB_SOURCE} "${FILE_CONTENTS}")

    file(READ ${OUTPUT_PB_HEADER} FILE_CONTENTS)
    string(REPLACE "namespace public {" "namespace p_ublic_ {" FILE_CONTENTS "${FILE_CONTENTS}")
    string(REPLACE "tinkoff::public::" "tinkoff::p_ublic_::" FILE_CONTENTS "${FILE_CONTENTS}")
    file(WRITE ${OUTPUT_PB_HEADER} "${FILE_CONTENTS}")

    file(READ ${OUTPUT_GRPC_SOURCE} FILE_CONTENTS)
    string(REPLACE "namespace public {" "namespace p_ublic_ {" FILE_CONTENTS "${FILE_CONTENTS}")
    string(REPLACE "tinkoff::public::" "tinkoff::p_ublic_::" FILE_CONTENTS "${FILE_CONTENTS}")
    file(WRITE ${OUTPUT_GRPC_SOURCE} "${FILE_CONTENTS}")

    file(READ ${OUTPUT_GRPC_HEADER} FILE_CONTENTS)
    string(REPLACE "namespace public {" "namespace p_ublic_ {" FILE_CONTENTS "${FILE_CONTENTS}")
    string(REPLACE "tinkoff::public::" "tinkoff::p_ublic_::" FILE_CONTENTS "${FILE_CONTENTS}")
    file(WRITE ${OUTPUT_GRPC_HEADER} "${FILE_CONTENTS}")

    list(APPEND OUTPUT_PB_SOURCES ${OUTPUT_PB_SOURCE})
    list(APPEND OUTPUT_PB_HEADERS ${OUTPUT_PB_HEADER})
    list(APPEND OUTPUT_GRPC_SOURCES ${OUTPUT_GRPC_SOURCE})
    list(APPEND OUTPUT_GRPC_HEADERS ${OUTPUT_GRPC_HEADER})

endforeach (tink_proto ${tink_protos})

add_library(tink_grpc_proto
  ${OUTPUT_PB_SOURCES}
  ${OUTPUT_PB_HEADERS}
  ${OUTPUT_GRPC_SOURCES}
  ${OUTPUT_GRPC_HEADERS}
  )

target_link_libraries(tink_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB TARGET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/services/*.cpp")
file(GLOB TARGET_HDR "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/services/*.h")

add_library(${PROJECT_NAME} SHARED ${TARGET_SRC} ${TARGET_HDR})

generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/services
        ${CMAKE_CURRENT_SOURCE_DIR}/services
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    tink_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    )

add_subdirectory(samples)
