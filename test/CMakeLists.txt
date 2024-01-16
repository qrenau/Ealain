
# Functions.

function(add_simple_test tname)
    add_executable(${tname} ${tname}.cpp)
    target_link_libraries(${tname} Ealain)
    add_test(NAME ${tname} COMMAND ${tname})
endfunction()


# Actually add tests.
add_simple_test(t-bresenham)
add_simple_test(t-rasterize-polygon)
add_simple_test(t-visibility)
