function(create_test_config_file)
	set(TEST_DATA_PATH "${PROJECT_SOURCE_DIR}/data/")
	configure_file(${PROJECT_SOURCE_DIR}/config.h.in "${PROJECT_BINARY_DIR}/config.h")
	include_directories(${PROJECT_BINARY_DIR})
endfunction()

function(add_test_case EXECUTABLE_NAME SRC_NAME)
	add_executable(${EXECUTABLE_NAME} ${SRC_NAME})
	target_link_libraries(${EXECUTABLE_NAME} CausalTrailLib ${Boost_LIBRARIES} gtest gtest_main)
	add_test(NAME ${EXECUTABLE_NAME} COMMAND ${EXECUTABLE_NAME})
endfunction()

