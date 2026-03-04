if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(LIB_TYPE STATIC)
else()
  set(LIB_TYPE SHARED)
endif()

add_library(libdynlin ${LIB_TYPE}
  Dynlin.cxx
)

set_target_properties(libdynlin PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
  INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
  OUTPUT_NAME dynlin
)

target_include_directories(libdynlin PUBLIC
  ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(libdynlin PRIVATE
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
    -fvisibility=hidden
    -fomit-frame-pointer
    -fcf-protection=full
    -fstack-clash-protection
    -D_FORTIFY_SOURCE=3
    -DNDEBUG
  >
)

install(TARGETS libdynlin
  LIBRARY DESTINATION .
  ARCHIVE DESTINATION .
)

