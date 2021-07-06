cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(otp_over_smtp)

find_package(CURL 7.77.0)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(otp ${SRC_FILES})

find_path(CURL_INCLUDE_DIRS ...)

if((NOT CURL_INCLUDE_DIRS) OR (NOT EXISTS ${CURL_INCLUDE_DIRS}))
    message("unable to find curl")
    
    execute_process(COMMAND git submodule update --init -- external/curl
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    
    add_subdirectory(external/curl)
    
    set(CURL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/external/curl/include
                            CACHE PATH "curl include directory")
    
    install(DIRECTORY ${CURL_INCLUDE_DIRS}/curl DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build)
    
    #add_library(curl INTERFACE)
    target_include_directories(curl INTERFACE
                               $<BUILD_INTERFACE:${CURL_INCLUDE_DIRS}>
                               $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/build>)
    
    #add_subdirectory(external/curl)
    # install(TARGETS curl EXPORT my_export_set DESTINATION ${some_dest})
else()
    target_link_libraries(otp PUBLIC ${CURL_LIBRARIES})
endif()

    find_package(CURL 7.77.0)
    target_include_directories(otp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CURL_INCLUDE_DIRS})
    target_link_libraries(otp PUBLIC ${CURL_LIBRARIES})


target_compile_options(otp PRIVATE -w)

# https://www.foonathan.net/2016/07/cmake-dependency-handling/
# https://cmake.org/cmake/help/v3.21/module/FindCURL.html