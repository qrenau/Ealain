include_directories(.)

add_executable(example_drift_num example_drift_num.cpp)
target_link_libraries(example_drift_num Ealain)

add_executable(example_drift_bit example_drift_bit.cpp)
target_link_libraries(example_drift_bit Ealain)

