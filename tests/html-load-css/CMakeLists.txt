add_executable(html_load_css main.cpp)
target_link_libraries(html_load_css web-parser)

add_test(NAME html_load_css COMMAND html_load_css)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/page.html ${CMAKE_CURRENT_BINARY_DIR}/page.html COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/style.css ${CMAKE_CURRENT_BINARY_DIR}/style.css COPYONLY)