function(create_test_config_file)
	set(TEST_DATA_PATH "${PROJECT_SOURCE_DIR}/data/")
	configure_file(${CMAKE_SOURCE_DIR}/config.h.in "${PROJECT_BINARY_DIR}/config.h")
	include_directories(${PROJECT_BINARY_DIR})
endfunction()

