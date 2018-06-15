set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required( VERSION 2.8 )
PROJECT( egeshare )

INCLUDE_DIRECTORIES(${EGEMAIN_PATH})
INCLUDE_DIRECTORIES(${EGEMAIN_PATH}/libssrc/others/boost_1_66_0)

message ("${EGEMAIN_PATH}<<< egeShared source code ---------------EGEMAIN_PATH------------------------ <<<<")

set(EGESHARE_HEADERS
    ${EGESHARE_SOURCE_CODE}egeConfig.h
    ${EGESHARE_SOURCE_CODE}egeDebug.h
    ${EGESHARE_SOURCE_CODE}egeSocket.h			#wrap boost.asio
    ${EGESHARE_SOURCE_CODE}egeAction.h
    ${EGESHARE_SOURCE_CODE}egeEmscriptenMouse.h   #specific handler for emscripten (glfw3 doesn't work with all browsers)
	${EGESHARE_SOURCE_CODE}Composite.h
)

set(EGESHARE_SOURCES
    ${EGESHARE_SOURCE_CODE}egeConfig.cpp
    ${EGESHARE_SOURCE_CODE}egeSocket.cpp			#wrap boost.asio
    ${EGESHARE_SOURCE_CODE}egeAction.cpp
    ${EGESHARE_SOURCE_CODE}egeEmscriptenMouse.cpp
	${EGESHARE_SOURCE_CODE}Composite.cpp
)

# Define here the needed parameters
set (EGE_VERSION_MAJOR 1)
set (EGE_VERSION_MINOR 0)
set (EGE_VERSION_PATCH 0) # subversion revision?
set (EGE_VERSION ${EGE_VERSION_MAJOR}.${EGE_VERSION_MINOR}.${EGE_VERSION_PATCH})
set (EGE_SOVERSION 3)
set (PROJECT_VERSION "${EGE_VERSION}")

message ("${PROJECT_NAME}:> Linker using ${EGE_DESTINATION_PLATFORM_STR} setting for project : <<${PROJECT_NAME}>> ***")
message ("${PROJECT_NAME}:> L'idea è di mettere tutte le parti comuni e condivisi in questa libreria. ")
if(EGE_SYS_EMSCRIPTEN)
    message ("OPTION->EGE_SYS_EMSCRIPTEN")
	message ("${PROJECT_NAME}:> Libreria STATICA .bc (da linkare al programma)")
	SET(PROJECT_NAME_EXT ${PROJECT_NAME}.bc)	#Aggiunge suffisso estensione file
	ADD_EXECUTABLE(${PROJECT_NAME_EXT} ${EGESHARE_HEADERS} ${EGESHARE_SOURCES})

  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden" )
	SET_TARGET_PROPERTIES(${PROJECT_NAME_EXT} PROPERTIES COMPILE_FLAGS "${EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS} " )
	message("${PROJECT_NAME}:> ESEGUIBILE in : <<${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>>")
endif()

if(EGE_SYS_EMSCRIPTEN_SIM)
    message ("OPTION->EGE_SYS_EMSCRIPTEN_SIM")
	message ("${PROJECT_NAME}:> Libreria STATICA .a (da linkare al programma)")
	SET(PROJECT_NAME_EXT ${PROJECT_NAME})		#Aggiunge suffisso estensione file
	
	cmake_policy(PUSH)
	cmake_policy(SET CMP0002 OLD)
		# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden -g" )
		ADD_LIBRARY(${PROJECT_NAME_EXT} STATIC ${EGESHARE_HEADERS} ${EGESHARE_SOURCES})
		SET_TARGET_PROPERTIES(${PROJECT_NAME_EXT} PROPERTIES COMPILE_FLAGS "${EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS}" ) 
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} stdc++)

	cmake_policy(POP)
	message("${PROJECT_NAME}:> LIBRERIA in : <<${CMAKE_LIBRARY_OUTPUT_DIRECTORY}>>")
endif()


if(EGE_SYS_LINUX)
    message ("OPTION->EGE_SYS_LINUX")
	message ("${PROJECT_NAME}:> Libreria STATICA .a (da linkare al programma)")
	SET(PROJECT_NAME_EXT ${PROJECT_NAME})		#Aggiunge suffisso estensione file
	
	cmake_policy(PUSH)
	cmake_policy(SET CMP0002 OLD)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden -g" )
		ADD_LIBRARY(${PROJECT_NAME_EXT} STATIC ${EGESHARE_HEADERS} ${EGESHARE_SOURCES})
		SET_TARGET_PROPERTIES(${PROJECT_NAME_EXT} PROPERTIES COMPILE_FLAGS "${EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS}" ) 
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} stdc++)

	cmake_policy(POP)
	message("${PROJECT_NAME}:> LIBRERIA in : <<${CMAKE_LIBRARY_OUTPUT_DIRECTORY}>>")
endif()

message("${PROJECT_NAME}:> Cartella progetto: ${CMAKE_CURRENT_SOURCE_DIR}")
INSTALL( 	
	TARGETS ${PROJECT_NAME_EXT}
	RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
	ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}     
)





