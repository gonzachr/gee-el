# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (opengl)

# The main executable
add_executable(main
  src/main.cpp
  src/ibuffer.cpp
  src/ibuffer.h
  src/renderer.cpp
  src/renderer.h
  src/shader.cpp
  src/shader.h
  src/texture.cpp
  src/texture.h
  src/varray.cpp
  src/varray.h
  src/vblayout.h
  src/vbuffer.cpp
  src/vbuffer.h
  src/vendor/stb/stb_image.cpp
  src/vendor/stb/stb_image.h
)
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
