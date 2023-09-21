add_executable(TORPEDOES main.cpp)
target_include_directories(TORPEDOES PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(TORPEDOES engine)
