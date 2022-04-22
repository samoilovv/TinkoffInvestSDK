cmake_minimum_required(VERSION 3.11)

project(tinkoffInvest)

find_package(Qt5 REQUIRED COMPONENTS Core Sql)

add_library(${PROJECT_NAME} MODULE
    tinkoffInvestUtils.h
    tinkoffInvestConsts.h
    tinkoffInvestPlugin.h
    tinkoffInvestPlugin.cpp
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
    hw_grpc_proto
    SDK
    )

# Include generated *.pb.h files
include_directories(${CMAKE_CURRENT_BINARY_DIR}/SDK)

set_plugin_dir(${PROJECT_NAME})

add_subdirectory(SDK)

