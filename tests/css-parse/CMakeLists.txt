add_executable(css_parse main.cpp)
target_link_libraries(css_parse web-parser)

add_test(NAME css_parse COMMAND css_parse)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/style.css ${CMAKE_CURRENT_BINARY_DIR}/style.css COPYONLY)