cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(
    FATAL_ERROR 
	"Please perform an out of tree build"
  )
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

project(
  Rylang
  VERSION 0.0.1 
  LANGUAGES C
)

if(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
  if(WIN32 AND NOT MINGW)
    message(
      FATAL_ERROR
      "[RYLANG] -- On windows, install and use clang > 6 supporting GNU ABI"
    )
  endif(WIN32 AND NOT MINGW)

  set(
    RYLANG_SOURCES 
    src/common.c
    src/rylang.c
  )
  
  add_executable(
    rylang 
    ${RYLANG_SOURCES}
  )
  
  target_compile_definitions(
    rylang 
    PRIVATE 
    $<$<CONFIG:Release>:NDEBUG>
  )
  
  target_include_directories(
    rylang
    PRIVATE
    ${Rylang_SOURCE_DIR}/include
  )

  target_compile_options(
    rylang
	PRIVATE
    -Wall -Wextra -Wpedantic -Wfloat-equal -Wunreachable-code -Wshadow
	-Wno-language-extension-token -Wno-nullability-extension 
	-Wno-incompatible-pointer-types -Wno-gnu-zero-variadic-macro-arguments
	-Wno-gnu-case-range
  )

  target_compile_options(
    rylang 
    PRIVATE 
    $<$<CONFIG:Debug>:-g -fno-omit-frame-pointer -fno-optimize-sibling-calls>
  )
  
  target_compile_options(
    rylang 
	PRIVATE 
    $<$<CONFIG:Release>:-Ofast>
  )
else(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
  message(
    FATAL_ERROR
	"[RYLANG] -- Expected clang compiler. Recieved ${CMAKE_C_COMPILER_ID}"
  )
endif(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")

include(CTest)
set(
  rylang_test_suites
  common
)
foreach(rylang_test_suite ${rylang_test_suites})
  list(APPEND rylang_test_sources tests/${rylang_test_suite}TestSuite.c)
endforeach(rylang_test_suite)

set(
  common_test_names
  test_common_buf_empty_cap_0
  test_common_buf_empty_len_0
  test_common_buf_push_10_elems_len_10
  test_common_buf_push_10_elems_right_order
  test_common_buf_free_sets_to_null
)

create_test_sourcelist(
  rylang_test_sourcelist
  rylang_test_driver.c 
  ${rylang_test_sources}
)
add_executable(
  rylang_test_driver 
  ${rylang_test_sourcelist} 
  src/common.c
)
add_custom_command(
  TARGET rylang_test_driver
  POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${Rylang_SOURCE_DIR}/tests/test-assets ${Rylang_BINARY_DIR}/test-assets
)
target_include_directories(
  rylang_test_driver 
  PRIVATE 
  ${Rylang_SOURCE_DIR}/include
  ${Rylang_SOURCE_DIR}/tests
)
target_compile_options(
  rylang_test_driver
  PRIVATE
  -Wno-incompatible-pointer-types
)

foreach(rylang_test_suite ${rylang_test_suites})
  foreach(rylang_test_name ${${rylang_test_suite}_test_names})
    add_test(
	  NAME ${rylang_test_name}
	  COMMAND rylang_test_driver tests/${rylang_test_suite}TestSuite 
	    ${rylang_test_name}
    )
  endforeach(rylang_test_name)
endforeach(rylang_test_suite)
