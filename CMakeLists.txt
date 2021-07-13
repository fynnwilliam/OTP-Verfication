cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(otp_over_smtp)

message(STATUS "Fetching curl")
include(FetchContent)
FetchContent_Declare(curl GIT_REPOSITORY https://github.com/curl/curl.git)
FetchContent_MakeAvailable(curl)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(otp ${SRC_FILES})

target_include_directories(otp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(otp PUBLIC CURL::libcurl)

target_compile_options(otp PRIVATE -w)