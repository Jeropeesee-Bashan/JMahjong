cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(jmahjong)
set(CMAKE_C_STANDARD 99)

find_package(Python 3 COMPONENTS Interpreter)

add_library(lib_${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/src/layout.c
  ${PROJECT_SOURCE_DIR}/src/mahjong.c
)
set_target_properties(lib_${PROJECT_NAME}
  PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.c)
target_include_directories(lib_${PROJECT_NAME} PUBLIC ./include)
target_link_libraries(${PROJECT_NAME} lib_${PROJECT_NAME})

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/layouts)
add_custom_target(layouts
  COMMAND ${Python_EXECUTABLE} ${PROJECT_SOURCE_DIR}/layouts/text_to_binary.py
                               ${PROJECT_SOURCE_DIR}/layouts/spider.txt
                               ${PROJECT_BINARY_DIR}/layouts/spider
  DEPENDS ${PROJECT_SOURCE_DIR}/layouts/spider.txt
)
add_dependencies(${PROJECT_NAME} layouts)
