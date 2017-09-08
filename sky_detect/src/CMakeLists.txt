include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( sky-detect sky-detect.cpp )
target_link_libraries( sky-detect ${OpenCV_LIBS} )
