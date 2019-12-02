cmake_minimum_required(VERSION 3.10)

if(WIN32)
	add_compile_options("/std:c++latest")
endif(WIN32)

if(APPLE)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CPP_COMPILER clang++)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD c++17)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY libc++)
    set(CMAKE_OSX_ARCHITECTURES x86_64)
    set(CMAKE_LIBRARY_PREFIX /opt/local /usr/local)
endif(APPLE)

project(Vector3vsVector4)

add_executable(test
	${PROJECT_SOURCE_DIR}/Sources/main.cpp
)
