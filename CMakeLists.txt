cmake_minimum_required(VERSION 3.3)

project(mmocli)

find_package(Boost REQUIRED)

find_package(GTest REQUIRED)

#------------------------------------------------------------------------------
# SERVER

set(SRC
  tcp_server.hpp
  tcp_server.cpp
  log.hpp
  log.cpp
  launch.hpp
  launch.cpp
)

add_executable(mmocli-server main_server.cpp ${SRC})

target_include_directories(mmocli-server PUBLIC ${Boost_INCLUDE_DIRS})

set(REQUIRED_STANDARD 23)
set_property(TARGET mmocli-server PROPERTY CXX_STANDARD ${REQUIRED_STANDARD})
set_property(TARGET mmocli-server PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_definitions(mmocli-server PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
target_compile_definitions(mmocli-server PRIVATE BOOST_BIND_NO_PLACEHOLDERS)
target_compile_definitions(mmocli-server PRIVATE _WIN32_WINNT=0x0A00)

#------------------------------------------------------------------------------
# TESTS

add_executable(tests main_tests.cpp ${SRC})

target_include_directories(tests PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(tests PUBLIC ${GTEST_INCLUDE_DIRS})

set(REQUIRED_STANDARD 23)
set_property(TARGET tests PROPERTY CXX_STANDARD ${REQUIRED_STANDARD})
set_property(TARGET tests PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_definitions(tests PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
target_compile_definitions(tests PRIVATE BOOST_BIND_NO_PLACEHOLDERS)
target_compile_definitions(tests PRIVATE _WIN32_WINNT=0x0A00)

target_link_libraries(tests ${GTEST_LIBRARIES})
