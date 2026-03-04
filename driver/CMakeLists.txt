add_executable(dynlin ${CMAKE_SOURCE_DIR}/driver/main.cxx)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  set_target_properties(dynlin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
    POSITION_INDEPENDENT_CODE ON
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH ON
  )
else()
  set_target_properties(dynlin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
    POSITION_INDEPENDENT_CODE OFF
  )
endif()

target_include_directories(dynlin PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(dynlin PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -Werror
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
  $<$<CONFIG:Release>:
    -O3
    -fomit-frame-pointer
    -fcf-protection=full
    -fstack-clash-protection
    -D_FORTIFY_SOURCE=3
    -DNDEBUG
  >
)

target_link_options(dynlin PRIVATE
  $<$<CONFIG:Release>:
    -static
  >
  $<$<NOT:$<CONFIG:Release>>:
    -Wl,-z,relro,-z,now
  >
)

target_link_libraries(dynlin PRIVATE
  libdynlin
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_custom_command(
    TARGET dynlin POST_BUILD
    COMMAND /usr/bin/strip "$<TARGET_FILE:dynlin>"
    COMMENT "Stripping symbols..."
  )
endif()

install(TARGETS dynlin
  RUNTIME DESTINATION .
)

