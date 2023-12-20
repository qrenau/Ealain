
file(GLOB core   "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB detection "${CMAKE_CURRENT_SOURCE_DIR}/detection/*.cpp")
file(GLOB map    "${CMAKE_CURRENT_SOURCE_DIR}/map/*.cpp")

add_library(ealain STATIC ${core} ${detection} ${map})

