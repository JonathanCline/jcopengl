cmake_minimum_required(VERSION 3.18)

project(jcopengl VERSION 0.1.0)




add_library(${PROJECT_NAME} STATIC "source/source.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC "include" PRIVATE "source")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_compile_definitions(${PROJECT_NAME} PUBLIC
	JCLIB_OPENGL_GL_VERSION_MAJOR=4 JCLIB_OPENGL_GL_VERSION_MINOR=5
	JCLIB_OPENGL_CUSTOM_LOADER_V=false)



#
# Link the glad interface target
#

if (NOT TARGET glad)
	message(FATAL_ERROR "please create an INTERFACE library target for glad")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC glad)


# Find and link OpenGL
#find_package(OpenGL "${__openglVersionMajor}.${__openglVersionMinor}" REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL)
	
# Trickle down!
ADD_CMAKE_SUBDIRS_HERE()


if (TARGET glm)
	target_link_libraries(${PROJECT_NAME} PUBLIC glm)
	target_compile_definitions(${PROJECT_NAME} PUBLIC JCLIB_OPENGL_GLM_V=true)
endif()


add_library(jclib::gl ALIAS ${PROJECT_NAME})
