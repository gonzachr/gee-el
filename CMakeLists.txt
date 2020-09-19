# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (opengl)

file(GLOB ROOT_SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB ROOT_HEADER ${PROJECT_SOURCE_DIR}/src/*.h)

file(GLOB ROOT_SOURCE_VENDOR ${PROJECT_SOURCE_DIR}/src/vendor/**/*.cpp)
file(GLOB ROOT_HEADER_VENDOR ${PROJECT_SOURCE_DIR}/src/vendor/**/*.h)

# The main executable
add_executable(main ${ROOT_SOURCE} ${ROOT_HEADER} ${ROOT_SOURCE_VENDOR} ${ROOT_HEADER_VENDOR})
set_property(TARGET main PROPERTY CXX_STANDARD 11)
target_compile_options(main PRIVATE -Wall)

# glfw
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE libglew_static)

# glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glm)

# imgui 
add_subdirectory(lib/imgui EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE imgui)