cmake_minimum_required (VERSION 3.8.2)
project(imgui VERSION 1.73.0 LANGUAGES CXX C)

add_library(imgui
  imgui.cpp
  imgui_draw.cpp
  imgui_widgets.cpp
)

add_library(imgui::imgui ALIAS imgui)

target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(imgui PRIVATE cxx_std_98)