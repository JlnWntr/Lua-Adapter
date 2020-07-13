cmake_minimum_required(VERSION 3.10)
include(ExternalProject)

set(LUA_VERSION "lua-5.4.0")
set(LUA_URL  "https://www.lua.org/ftp/${LUA_VERSION}.tar.gz")

ExternalProject_Add(lua
  URL ${LUA_URL}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make generic
  BUILD_ALWAYS true
  BUILD_IN_SOURCE true
  INSTALL_COMMAND ""
)

add_library(liblua STATIC IMPORTED)

ExternalProject_Get_property(lua SOURCE_DIR)
set(LIBLUA_DIR "${SOURCE_DIR}/src")

# Project to test liblua
project(test_lua)
set_property(TARGET liblua PROPERTY IMPORTED_LOCATION ${LIBLUA_DIR}/liblua.a)

add_executable(test_lua test.cpp MiniLua.cpp)
target_link_libraries(test_lua liblua)
target_include_directories(test_lua PUBLIC ${LIBLUA_DIR})
set_property(TARGET test_lua PROPERTY CXX_STANDARD 11)
set_property(TARGET test_lua PROPERTY RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")

# Test
add_test(NAME Run COMMAND "./test_lua")
ENABLE_TESTING()