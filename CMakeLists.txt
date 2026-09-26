# ESP32-S3 BLE信标基站 - 主CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 设置需要排除的组件（避免Git子模块问题）
set(EXCLUDE_COMPONENTS unity cmock spiffs)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(Beacon)
