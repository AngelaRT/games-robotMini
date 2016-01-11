set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

#add_executable(alz_memory_game.exe 
#				alz_memory_game.cpp alz_memory_game.h)
add_executable(alz_memory_game.exe 
				game_node.h game_classes.h
				game_common.cpp game_node.cpp
				game_XML.cpp game_Browser.cpp)
target_link_libraries(alz_memory_game.exe 
						xml_document speech_snippets ${catkin_LIBRARIES})

add_executable(game_handler.exe
								game_handler.cpp game_handler.h)
target_link_libraries(game_handler.exe 
						xml_document ${catkin_LIBRARIES})

