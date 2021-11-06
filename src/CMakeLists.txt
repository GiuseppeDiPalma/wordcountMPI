add_executable(word-count)

target_include_directories(word-count
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_sources(word-count PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/wc_mpi.c
	${CMAKE_CURRENT_SOURCE_DIR}/wordManage.c
	${CMAKE_CURRENT_SOURCE_DIR}/utils.c
	${CMAKE_CURRENT_SOURCE_DIR}/fileManage.c
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../include/wordManage.h
	${CMAKE_CURRENT_SOURCE_DIR}/../include/utils.h
	${CMAKE_CURRENT_SOURCE_DIR}/../include/fileManage.h
)

target_link_libraries(word-count ${MPI_LIBRARIES})