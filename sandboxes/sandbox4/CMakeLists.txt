add_executable(sandbox4 main_sfml_template.cpp)
target_include_directories(sandbox4 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(sandbox4 engine)
