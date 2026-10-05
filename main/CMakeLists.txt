// File: main/CMakeLists.txt
#include "idf.def.h"

// 登记 main.c 源文件，并声明依赖的组件 (如果存在)
idf_component_register(SRCS "main.c" INCLUDE_DIRS ".")