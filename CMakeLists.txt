cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_CXX_STANDARD 20)

project(otp_over_smtp)

add_executable(otp src/main.cpp src/otp.cpp src/smtp.cpp)

target_link_libraries(otp -lcurl)

target_compile_options(otp PRIVATE -w)