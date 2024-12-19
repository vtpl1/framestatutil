# *****************************************************
#    Copyright 2022 Videonetics Technology Pvt Ltd
# *****************************************************



add_executable(tests_framestatutil
	src/test.cpp
)

target_link_libraries(tests_framestatutil
	PRIVATE framestatutil
)
