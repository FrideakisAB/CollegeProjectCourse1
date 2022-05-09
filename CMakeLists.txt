cmake_minimum_required(VERSION 3.16)

project(PhysEng CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(include)

find_package(OpenGL REQUIRED)

# Исправление Conan при компиляции Clang под MSVC
set(PROJECT_IS_MSVC_CLANG (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT} STREQUAL "MSVC"))

if(PROJECT_IS_MSVC_CLANG AND NOT CMAKE_CXX_COMPILER_ID MATCHES GNU)
	set(PREV_CMAKE_CXX_COMPILER_ID ${CMAKE_CXX_COMPILER_ID})
	set(PREV_CMAKE_CXX_GENERATOR_TOOLSET ${CMAKE_CXX_GENERATOR_TOOLSET})
	set(CMAKE_CXX_COMPILER_ID MSVC)
	set(CMAKE_CXX_GENERATOR_TOOLSET ClangCL)
	
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHs")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /EHs")
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
	message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.17.0/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt 
	INSTALL_FOLDER ${CMAKE_BINARY_DIR}/pkgs
	BASIC_SETUP
	BUILD missing
)

if(PROJECT_IS_MSVC_CLANG AND NOT CMAKE_CXX_COMPILER_ID MATCHES GNU)
	set(CMAKE_CXX_COMPILER_ID ${PREV_CMAKE_CXX_COMPILER_ID})
	set(CMAKE_CXX_GENERATOR_TOOLSET ${PREV_CMAKE_CXX_GENERATOR_TOOLSET})
endif()

set(Source
	src/main.cpp
	src/Timer.cpp
	src/Engine.cpp
	src/ResourceManager.cpp

	src/Game/Sprite.cpp
	src/Game/Simulation.cpp

	src/Game/UI/InfoWindow.cpp
	src/Game/UI/ControlWindow.cpp

	src/Render/Render.cpp
	src/Render/Shader.cpp

	src/ImGui/imgui_impl_glfw.cpp
	src/ImGui/imgui_impl_opengl3.cpp
	
	src/prog.rc
)

add_executable(PhysEngApp WIN32
	${Source}
)

target_link_libraries(PhysEngApp
	OpenGL::GL
	${CONAN_LIBS}
)
