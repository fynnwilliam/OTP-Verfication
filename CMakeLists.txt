cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(otp_over_smtp)

find_package(CURL REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(otp ${SRC_FILES})
target_include_directories(otp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CURL_INCLUDE_DIRS})

target_link_libraries(otp PUBLIC ${CURL_LIBRARIES})

target_compile_options(otp PRIVATE -w)