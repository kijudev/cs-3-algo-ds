cmake_minimum_required(VERSION 4.0)

project(cs-3-algo-ds LANGUAGES C)

# Standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Warnings
add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  -Werror
  -Wshadow
  -Wconversion
  -Wsign-conversion
  -Wcast-align
  -Wcast-qual
  -Wnull-dereference
  -Wdouble-promotion
  -Wformat=2
  -Wimplicit-fallthrough
  -Wmisleading-indentation
)

# Sanitizers
option(ENABLE_ASAN  "Enable AddressSanitizer"           OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_MSAN  "Enable MemorySanitizer"            OFF)

if(ENABLE_ASAN AND ENABLE_MSAN)
  message(FATAL_ERROR "ASan and MSan are mutually exclusive.")
endif()

if(ENABLE_ASAN OR ENABLE_UBSAN OR ENABLE_MSAN)
  add_compile_options(-fno-omit-frame-pointer -fno-optimize-sibling-calls)
endif()

if(ENABLE_ASAN)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

if(ENABLE_UBSAN)
  add_compile_options(-fsanitize=undefined)
  add_link_options(-fsanitize=undefined)
endif()

if(ENABLE_MSAN)
  add_compile_options(-fsanitize=memory -fsanitize-memory-track-origins=2)
  add_link_options(-fsanitize=memory)
endif()

# One executable per problem-*/main.c, named after its folder
file(GLOB PROBLEM_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/problem-*/main.c)

foreach(source ${PROBLEM_SOURCES})
  get_filename_component(dir ${source} DIRECTORY)
  get_filename_component(name ${dir} NAME)
  add_executable(${name} ${source})
endforeach()
