cmake_minimum_required(VERSION 3.22)

project(mmocli)

find_package(Boost REQUIRED)

find_package(GTest REQUIRED)

#------------------------------------------------------------------------------
# LIB

set(SRC
    config.hpp
    log.hpp
    log.cpp
    on_exit.hpp
    tcp_server.hpp
    tcp_server.cpp
    tcp_client.hpp
    tcp_client.cpp
    game_server.hpp
    game_server.cpp
    server.cpp
    server.hpp
)

add_library(mmocli SHARED ${SRC})

target_include_directories(mmocli PUBLIC ${Boost_INCLUDE_DIRS})

set(REQUIRED_STANDARD 23)
set_property(TARGET mmocli PROPERTY CXX_STANDARD ${REQUIRED_STANDARD})
set_property(TARGET mmocli PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_definitions(mmocli PRIVATE MMOCLI_BUILDING_THE_LIB)
target_compile_definitions(mmocli PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
target_compile_definitions(mmocli PRIVATE BOOST_BIND_NO_PLACEHOLDERS)
target_compile_definitions(mmocli PRIVATE _WIN32_WINNT=0x0A00)

#------------------------------------------------------------------------------
# SERVER

#add_executable(mmocli-server main_server.cpp)

#target_include_directories(mmocli-server PUBLIC ${Boost_INCLUDE_DIRS})

#set(REQUIRED_STANDARD 23)
#set_property(TARGET mmocli-server PROPERTY CXX_STANDARD ${REQUIRED_STANDARD})
#set_property(TARGET mmocli-server PROPERTY CXX_STANDARD_REQUIRED ON)

#target_compile_definitions(mmocli-server PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
#target_compile_definitions(mmocli-server PRIVATE BOOST_BIND_NO_PLACEHOLDERS)
#target_compile_definitions(mmocli-server PRIVATE _WIN32_WINNT=0x0A00)

#target_link_libraries(mmocli-server mmocli)


#------------------------------------------------------------------------------
# TESTS

add_executable(mmocli-tests main_tests.cpp)

target_include_directories(mmocli-tests PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(mmocli-tests PUBLIC ${GTEST_INCLUDE_DIRS})

set(REQUIRED_STANDARD 23)
set_property(TARGET mmocli-tests PROPERTY CXX_STANDARD ${REQUIRED_STANDARD})
set_property(TARGET mmocli-tests PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_definitions(mmocli-tests PRIVATE _WINSOCK_DEPRECATED_NO_WARNINGS)
target_compile_definitions(mmocli-tests PRIVATE BOOST_BIND_NO_PLACEHOLDERS)
target_compile_definitions(mmocli-tests PRIVATE _WIN32_WINNT=0x0A00)
target_compile_definitions(mmocli-tests PRIVATE MMOCLI_TESTS)

message(${GTEST_BOTH_LIBRARIES})
target_link_libraries(mmocli-tests ${GTEST_BOTH_LIBRARIES})

target_link_libraries(mmocli-tests mmocli)
