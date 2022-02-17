add_library(getopt
	getopt.c
)
target_include_directories(getopt PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
)
