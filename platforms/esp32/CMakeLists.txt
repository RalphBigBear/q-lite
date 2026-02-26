# Q-Lite ESP32 Project CMakeLists.txt
# Supports: ESP32, ESP32-S3, ESP32-C3

cmake_minimum_required(VERSION 3.5)

# Include ESP-IDF project
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set target chip (default: ESP32-S3)
if(NOT DEFINED IDF_TARGET)
    set(IDF_TARGET "esp32s3")
endif()

# Project definition
project(q-lite-esp32)
