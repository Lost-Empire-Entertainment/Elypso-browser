cmake_minimum_required(VERSION 3.29.5)

# Ensure install path is set correctly
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out/install")
endif()

if (WIN32)
    # Enable hot reload for MSVC
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
    # Explicitly set the MSVC toolset for Visual Studio
    if (MSVC)
        set(CMAKE_GENERATOR_TOOLSET "v143")
    endif()
endif()

# Specify the compiler for Linux builds
if (UNIX)
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_MAJOR_VERSION 1)
set(PROJECT_MINOR_VERSION 0)
set(PROJECT_PATCH_VERSION 0)
set(PROJECT_VERSION "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION}")

project("Elypso Browser" VERSION ${PROJECT_VERSION})

# Set runtime library type for MSVC
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Paths to frequently used directories
get_filename_component(PARENT_DIR "${CMAKE_SOURCE_DIR}" DIRECTORY)
set(PARENT_DIR_PATH "${PARENT_DIR}")

# Physics files
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# External files
set(EXT_GLFW_DIR "${CMAKE_SOURCE_DIR}/_external_shared/GLFW")
set(EXT_GLAD_DIR "${CMAKE_SOURCE_DIR}/_external_shared/Glad")
set(EXT_IMGUI_DIR "${CMAKE_SOURCE_DIR}/_external_shared/ImGui")

# Add GLAD
add_library(glad STATIC ${EXT_GLAD_DIR}/glad.c)
target_include_directories(glad PUBLIC ${EXT_GLAD_DIR}/..)

# Add GLFW
if (WIN32)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		set(GLFW_LIBRARY_PATH "${EXT_GLFW_DIR}/release/glfw3dll.lib")
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(GLFW_LIBRARY_PATH "${EXT_GLFW_DIR}/debug/glfw3dll.lib")
	endif()
else()
	find_package(glfw3 REQUIRED)
endif()

# Group external libraries
add_library(external_libs INTERFACE)
target_link_libraries(external_libs INTERFACE glad)

# Add source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    ${SRC_DIR}/*.cpp
    ${SRC_DIR}/*/*.cpp
)

# Add external source files
list(APPEND SOURCE_FILES
	${EXT_GLAD_DIR}/glad.c
	${EXT_IMGUI_DIR}/imgui.cpp
	${EXT_IMGUI_DIR}/imgui_draw.cpp
	${EXT_IMGUI_DIR}/imgui_impl_glfw.cpp
	${EXT_IMGUI_DIR}/imgui_impl_opengl3.cpp
	${EXT_IMGUI_DIR}/imgui_tables.cpp
	${EXT_IMGUI_DIR}/imgui_widgets.cpp
)

# Build as browser executable
add_executable(ElypsoBrowser ${SOURCE_FILES})

if (WIN32)
	set_source_files_properties(${RESOURCE_FILE} PROPERTIES LANGUAGE RC)
	target_sources(ElypsoBrowser PRIVATE ${RESOURCE_FILE})
endif()

# Forces the name to be Elypso browser
set_target_properties(ElypsoBrowser PROPERTIES OUTPUT_NAME "Elypso browser")

target_compile_features(ElypsoBrowser PRIVATE cxx_std_20)

# Apply all directories to target
target_include_directories(ElypsoBrowser PRIVATE
    ${INCLUDE_DIR}
    ${EXT_GLFW_DIR}
	${EXT_GLAD_DIR}
	${EXT_IMGUI_DIR}
)

if (WIN32)
	target_link_libraries(ElypsoBrowser PRIVATE external_libs ${GLFW_LIBRARY_PATH})
else()
	find_package(Wayland QUIET)
	find_package(X11 QUIET)
	if (Wayland_FOUND)
		target_link_libraries(ElypsoBrowser PRIVATE external_libs glfw Wayland::Client)
	elseif (X11_FOUND)
		target_link_libraries(ElypsoBrowser PRIVATE external_libs glfw ${X11_LIBRARIES})
	endif()
endif()

# Define GLFW_INCLUDE_NONE
target_compile_definitions(ElypsoBrowser PRIVATE GLFW_INCLUDE_NONE)

# Set the folder inside the install folder where the exe will be placed for this project
set(CMAKE_INSTALL_BINDIR bin)
install(TARGETS ElypsoBrowser DESTINATION ${CMAKE_INSTALL_BINDIR})

# Copy GLFW DLL to the install directory after build
if (WIN32)
    set(GLFW_DLL_ORIGIN_BASE "${CMAKE_SOURCE_DIR}/files/external dlls")
	set(GLFW_DLL_TARGET "$<TARGET_FILE_DIR:ElypsoBrowser>/glfw3.dll")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(GLFW_DLL_ORIGIN "${GLFW_DLL_ORIGIN_BASE}/release/glfw3.dll")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(GLFW_DLL_ORIGIN "${GLFW_DLL_ORIGIN_BASE}/debug/glfw3.dll")
    endif()

    add_custom_command(TARGET ElypsoBrowser POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove 
		${GLFW_DLL_TARGET}
        COMMAND ${CMAKE_COMMAND} -E copy 
		${GLFW_DLL_ORIGIN}
		${GLFW_DLL_TARGET}
    )
endif()

set(COPY_TARGETS LICENSE.md README.md SECURITY.md LIBRARIES.md)
foreach(FILE ${COPY_TARGETS})
  add_custom_command(TARGET ElypsoBrowser POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E remove
	$<TARGET_FILE_DIR:ElypsoBrowser>/${FILE}
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/${FILE}
	$<TARGET_FILE_DIR:ElypsoBrowser>/${FILE}
  )
endforeach()

# Include CPack to enable packaging
include(CPack)