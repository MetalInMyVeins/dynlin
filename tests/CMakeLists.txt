find_package(GTest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

add_executable(dynlin_tests
  test_dynlin.cxx
)

target_include_directories(dynlin_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(dynlin_tests PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
  -Wconversion
  -Wsign-conversion
  -Wformat=2
  -Wundef
  -Wcast-align
  -Wcast-qual
  -Wdouble-promotion
  -Wnull-dereference
  -Wold-style-cast
  -Woverloaded-virtual
  -Wnon-virtual-dtor
  -Wimplicit-fallthrough
  -Wredundant-decls
  -Wswitch-enum
  -Wzero-as-null-pointer-constant
  -fno-exceptions
  -fno-rtti
)

target_link_libraries(dynlin_tests PRIVATE
  gtest
  gtest_main
)

set_target_properties(dynlin_tests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

include(GoogleTest)
gtest_discover_tests(dynlin_tests
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
  PROPERTIES
    LABELS "unit"
)

