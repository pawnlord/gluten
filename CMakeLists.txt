cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project ("gluten")

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include(CMakePrintHelpers)
cmake_print_variables(GLEW_INCLUDE_DIRS GLEW_LIBRARIES)

add_compile_definitions(PROJECT_NAME=\"${PROJECT_NAME}\")

add_library(glfw STATIC IMPORTED)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/assimp)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-D_CRT_SECURE_NO_WARNINGS
)

set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/glfw/lib-vc2022/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/lib/glfw/include"
)

# set_target_properties(glew PROPERTIES
#     IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/glew/lib/Release/x64/glew32.lib"
#     INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/lib/glew/include"
# )

include_directories(${PROJECT_SOURCE_DIR}/lib/glm/)
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/stb)
include_directories(${PROJECT_SOURCE_DIR}/include)
# include_directories(${PROJECT_SOURCE_DIR}/lib/glew/include)
set(GLM_TEST_ENABLE OFF CACHE BOOL "GLM Build unit tests")

add_executable(${PROJECT_NAME} source/main.cpp
                               source/Camera.cpp
                               source/GraphicsContext.cpp
                               source/GraphicsManager.cpp
                               source/InputManager.cpp
                               source/ResourceManager.cpp
                               source/Shader.cpp
                               source/ObjectGenerator.cpp)

include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} assimp)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/source/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
