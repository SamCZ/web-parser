add_executable(html_parse main.cpp)
target_link_libraries(html_parse web-parser)

add_test(NAME html_parse COMMAND html_parse)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/page.html ${CMAKE_CURRENT_BINARY_DIR}/page.html COPYONLY)