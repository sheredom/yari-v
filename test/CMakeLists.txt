# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.
#
# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# For more information, please refer to <http://unlicense.org/>

project(yariv)
cmake_minimum_required(VERSION 2.8.7)

file(GLOB_RECURSE shaders ${CMAKE_CURRENT_SOURCE_DIR}/external/*)
list(LENGTH shaders shaders_length)

set(header ${CMAKE_CURRENT_BINARY_DIR}/shaders.h)

file(WRITE ${header} "#define SHADERS_LENGTH ${shaders_length}\nconst char* shaders[SHADERS_LENGTH] = {\n")

foreach(shader IN LISTS shaders)
  file(APPEND ${header} "  \"${shader}\",\n")
endforeach()

file(APPEND ${header} "};\n")

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(yariv_test
  ${CMAKE_CURRENT_SOURCE_DIR}/../yariv.h
  ${CMAKE_CURRENT_BINARY_DIR}/shaders.h
  ${CMAKE_CURRENT_SOURCE_DIR}/utest.h
  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/test.c
  ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set_source_files_properties(test.c test.cpp PROPERTIES
    COMPILE_FLAGS "-Wall -Wextra -Werror"
  )
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set_source_files_properties(test.c test.cpp PROPERTIES
    COMPILE_FLAGS "-Wall -Wextra -Weverything -Werror"
  )
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  set_source_files_properties(test.c test.cpp PROPERTIES
    COMPILE_FLAGS "/Wall /WX /wd4514"
  )
else()
  message(WARNING "Unknown compiler '${CMAKE_C_COMPILER_ID}'!")
endif()
