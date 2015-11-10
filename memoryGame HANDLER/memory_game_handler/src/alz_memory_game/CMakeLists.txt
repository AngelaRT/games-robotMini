

add_executable(alz_memory_game.exe 
				alz_memory_game.cpp alz_memory_game.h)

target_link_libraries(alz_memory_game.exe 
						xml_document speech_snippets ${catkin_LIBRARIES})
