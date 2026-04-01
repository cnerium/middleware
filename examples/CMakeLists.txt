# ========================
# Examples - cnerium/middleware
# ========================

add_executable(cnerium_middleware_example_01 01_basic.cpp)
target_link_libraries(cnerium_middleware_example_01 PRIVATE cnerium::middleware)
target_compile_features(cnerium_middleware_example_01 PRIVATE cxx_std_20)

add_executable(cnerium_middleware_example_02 02_chain.cpp)
target_link_libraries(cnerium_middleware_example_02 PRIVATE cnerium::middleware)
target_compile_features(cnerium_middleware_example_02 PRIVATE cxx_std_20)

add_executable(cnerium_middleware_example_03 03_short_circuit.cpp)
target_link_libraries(cnerium_middleware_example_03 PRIVATE cnerium::middleware)
target_compile_features(cnerium_middleware_example_03 PRIVATE cxx_std_20)

add_executable(cnerium_middleware_example_04 04_full_demo.cpp)
target_link_libraries(cnerium_middleware_example_04 PRIVATE cnerium::middleware)
target_compile_features(cnerium_middleware_example_04 PRIVATE cxx_std_20)
