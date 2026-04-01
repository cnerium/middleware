# ========================
# Examples - cnerium/middleware
# ========================

add_executable(middleware_example_01 01_basic.cpp)
target_link_libraries(middleware_example_01 PRIVATE middleware::middleware)

add_executable(middleware_example_02 02_chain.cpp)
target_link_libraries(middleware_example_02 PRIVATE middleware::middleware)

add_executable(middleware_example_03 03_short_circuit.cpp)
target_link_libraries(middleware_example_03 PRIVATE middleware::middleware)

add_executable(middleware_example_04 04_full_demo.cpp)
target_link_libraries(middleware_example_04 PRIVATE middleware::middleware)
