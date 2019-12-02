cmake_minimum_required(VERSION 3.10)

if(WIN32)
	add_compile_options("/std:c++latest")
endif(WIN32)

project(Vector3vsVector4)

add_executable(test
	${PROJECT_SOURCE_DIR}/Sources/main.cpp
)