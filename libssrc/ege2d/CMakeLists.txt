set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required( VERSION 2.8 )
PROJECT( ege2d )

#Condividiamo le librerie in comune messe a disposizione da egeimp (es. 3DSHelper.h e dipendenze)
INCLUDE_DIRECTORIES(${EGESHARE_SOURCE_CODE})

INCLUDE_DIRECTORIES(${EGE_INCLUDE_FREETYPE2})		

INCLUDE_DIRECTORIES(${EGE2D_INCLUDE_CODE})		
INCLUDE_DIRECTORIES(${EGE2D_SOURCE_CODE})		

INCLUDE_DIRECTORIES(${EGE_INCLUDE_JPEG})		
INCLUDE_DIRECTORIES(${EGE_INCLUDE_GLFW})		

set(EGE_HEADERS
    ${EGE2D_INCLUDE_CODE}EGE3dOpenGLInterface.h
	${EGE2D_INCLUDE_CODE}EGE3dGraphicContext.h
	${EGE2D_INCLUDE_CODE}EGE3dTextures.h
	${EGE2D_INCLUDE_CODE}EGE3dBaseTypes.h

	${EGE2D_INCLUDE_CODE}EGE2dFramebuffer.h					# 2D !!!
	${EGE2D_INCLUDE_CODE}EGE2dDisplay.h
	${EGE2D_INCLUDE_CODE}EGE2dComposition.h
	${EGE2D_INCLUDE_CODE}EGE2dTools.h
	${EGE2D_INCLUDE_CODE}EGE2d-freetype.h

	#${EGESHARE_SOURCE_CODE}egeSocket.h
)

set(EGE_SOURCES
	${EGE2D_INCLUDE_CODE}EGE3dOpenGLInterface.cpp
	${EGE2D_INCLUDE_CODE}EGE3dGraphicContext.cpp
	${EGE2D_INCLUDE_CODE}EGE3dTextures.cpp
	${EGE2D_INCLUDE_CODE}EGE3dErrorHandler.cpp

	${EGE2D_SOURCE_CODE}EGE2dFramebuffer.cpp				# 2D !!!
    ${EGE2D_INCLUDE_CODE}EGE2dDisplay.cpp
	${EGE2D_INCLUDE_CODE}EGE2dComposition.cpp
	${EGE2D_INCLUDE_CODE}EGE2dTools.cpp
	${EGE2D_INCLUDE_CODE}EGE2d-freetype.cpp

	#${EGESHARE_SOURCE_CODE}egeSocket.cpp
)

# Define here the needed parameters
set (EGE_VERSION_MAJOR 0)
set (EGE_VERSION_MINOR 0)
set (EGE_VERSION_PATCH 1) # subversion revision?
set (EGE_VERSION ${EGE_VERSION_MAJOR}.${EGE_VERSION_MINOR}.${EGE_VERSION_PATCH})
set (EGE_SOVERSION 3)
set (PROJECT_VERSION "${EGE_VERSION}")

INCLUDE_DIRECTORIES( include )

message ("${PROJECT_NAME}:> Linker using ${EGE_DESTINATION_PLATFORM_STR} setting for project : <<${PROJECT_NAME}>> ***")

if(EGE_SYS_EMSCRIPTEN)
	message ("${PROJECT_NAME}:> Libreria STATICA .bc (da linkare al programma)")
	SET(PROJECT_NAME_EXT ${PROJECT_NAME}.bc)	#Aggiunge suffisso estensione file
	ADD_EXECUTABLE(${PROJECT_NAME_EXT} ${EGE_HEADERS} ${EGE_SOURCES})
	
	#2018.02.22
	SET(EGE_EMSCRIPTEN_JAVASCRIPT_LIBS_PATH	/home/giovanni/NetBeansProjects/EGE/libssrc/others/emscripten-libs-js)
	#Collegamento a funzioni OpenGL messe a disposizione da EMSCRIPTEN (subset di ES2)
	SET(EGE_IMPORT_LIBRARY_gl "--js-library ${EGE_EMSCRIPTEN_JAVASCRIPT_LIBS_PATH}/library_gl.js")
	SET(EGE_IMPORT_LIBRARY_glfw "--js-library ${EGE_EMSCRIPTEN_JAVASCRIPT_LIBS_PATH}/library_glfw.js")
	SET(EGE_IMPORT_LIBRARY_glew "--js-library ${EGE_EMSCRIPTEN_JAVASCRIPT_LIBS_PATH}/library_glew.js")
#2018.02.22 - fine


  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden" )
	SET_TARGET_PROPERTIES(${PROJECT_NAME_EXT} PROPERTIES COMPILE_FLAGS "${EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS} " )
	message("${PROJECT_NAME}:> ESEGUIBILE in : <<${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>>")
endif()

if(EGE_SYS_LINUX)
    message ("OPTION->EGE_SYS_LINUX")
	message ("${PROJECT_NAME}:> Libreria STATICA .a (da linkare al programma)")
	SET(PROJECT_NAME_EXT ${PROJECT_NAME})		#Aggiunge suffisso estensione file
	
	cmake_policy(PUSH)
	cmake_policy(SET CMP0002 OLD)
        SET(EGELIBS ${EGEMAIN_PATH}/../../build/lib/unix)
		#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden -g" )
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g" )
		ADD_LIBRARY(${PROJECT_NAME_EXT} STATIC ${EGE_HEADERS} ${EGE_SOURCES})
		SET_TARGET_PROPERTIES(${PROJECT_NAME_EXT} PROPERTIES COMPILE_FLAGS "${EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS}" ) 
		TARGET_LINK_LIBRARIES(${PROJECT_NAME} stdc++ -L${EGELIBS} -legejpeg -legeshare -legefreetype)
        
	cmake_policy(POP)
	message("${PROJECT_NAME}:> LIBRERIA in : <<${CMAKE_LIBRARY_OUTPUT_DIRECTORY}>>")
endif()



##if(EGE_SYS_LINUX)
##	message ("${PROJECT_NAME}:> Libreria STATICA .a (da linkare al programma)")
##	SET(PROJECT_NAME_EXT ${PROJECT_NAME})		#Aggiunge suffisso estensione file
	
##    SET(EGELIBS /home/giovanni/NetBeansProjects/ege2d-master/build/lib/unix)
##	LINK_DIRECTORIES(${EGELIBS})
    
##    set(EGE_SOURCES_EXTRA_LINUX
##        jpgd.h
##    	jpge.h
##        jpgd.cpp
##	    jpge.cpp
##    )

##	cmake_policy(PUSH)
##	cmake_policy(SET CMP0002 OLD)
		#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden -g" )
##		ADD_LIBRARY(${PROJECT_NAME_EXT} STATIC ${EGE_HEADERS} ${EGE_SOURCES} $(EGE_SOURCES_EXTRA_LINUX))
##		SET_TARGET_PROPERTIES(${PROJECT_NAME_EXT} PROPERTIES COMPILE_FLAGS "${EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS}" ) 
##		TARGET_LINK_LIBRARIES(${PROJECT_NAME} stdc++ egejpeg egeshare freetype dl)

##	cmake_policy(POP)
##	message("${PROJECT_NAME}:> LIBRERIA in : <<${CMAKE_LIBRARY_OUTPUT_DIRECTORY}>>")
##endif()

message("${PROJECT_NAME}:> Cartella progetto: ${CMAKE_CURRENT_SOURCE_DIR}")
INSTALL( 	
	TARGETS ${PROJECT_NAME_EXT}
	RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
	ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}     
)





