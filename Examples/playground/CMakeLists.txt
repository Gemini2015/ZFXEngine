set(EXAMPLE_NAME "playground")

set(SRC_LIST
	main.cpp)

set(INC_LIST
	)

add_executable(${EXAMPLE_NAME} ${SRC_LIST} ${INC_LIST})
target_link_libraries(${EXAMPLE_NAME} ZFXEngine)

create_target_launcher(${EXAMPLE_NAME}
	WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/bin")