cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

add_subdirectory(opengl_viewer_3d)
add_subdirectory(gl_widget_2d)

set(target_gl_widget_2d ${target_gl_widget_2d} PARENT_SCOPE)
