add_executable(sandbox2 main_sfml_template.cpp)
target_link_libraries(sandbox2 engine)

# sandbox2 Header Files
#target_include_directories("${PROJECT_NAME}" PUBLIC
#"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Header_Files"
#"$<INSTALL_INTERFACE:Header Files>"
#)

#add_subdirectory("Header Files")
