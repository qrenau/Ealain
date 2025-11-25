include_directories(.)

add_executable(example_so example_so.cpp)
target_link_libraries(example_so Ealain)

add_executable(lon_omni_cameras lon_omni_cameras.cpp)
target_link_libraries(lon_omni_cameras Ealain)

add_executable(lon_uni_cameras lon_uni_cameras.cpp)
target_link_libraries(lon_uni_cameras Ealain)

add_executable(lon_walls_omni lon_walls_omni.cpp)
target_link_libraries(lon_walls_omni Ealain)

add_executable(lon_omni_constraint_out lon_omni_constraint_out.cpp)
target_link_libraries(lon_omni_constraint_out Ealain)
