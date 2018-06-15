CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#About this project
#----------------------------------
PROJECT(EGE2D-Tutorial03)
SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "0")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

message(">>>> ----------------------------------------------------------------------- <<<<")
message(">>>> TUTORIAL SOURCE DIRECTORY : " ${PROJECT_BINARY_DIR})
message(">>>> DESTINATION PLATFORM STR : " ${EGE_DESTINATION_PLATFORM_STR})
message(">>>> ----------------------------------------------------------------------- <<<<")

SET(EGEMAIN_PATH 	${PROJECT_BINARY_DIR})
INCLUDE_DIRECTORIES(${EGEMAIN_PATH})

SET(EGE2D_INCLUDE_CODE 		../../../libssrc/ege2d/)
SET(EGE2D_SOURCE_CODE 		../../../libssrc/ege2d/)
INCLUDE_DIRECTORIES(${EGE2D_INCLUDE_CODE})
INCLUDE_DIRECTORIES(${EGE2D_SOURCE_CODE})

#SET(EGE3D_INCLUDE_CODE 		../../../libssrc/ege3d/include/)
#SET(EGE3D_SOURCE_CODE 		../../../libssrc/ege3d/include/)
#INCLUDE_DIRECTORIES(${EGE3D_INCLUDE_CODE})
#INCLUDE_DIRECTORIES(${EGE3D_SOURCE_CODE})

SET(EGESHARE_INCLUDE_CODE 		../../../libssrc/egeshare/src/)
SET(EGESHARE_SOURCE_CODE 		../../../libssrc/egeshare/src/)
INCLUDE_DIRECTORIES(${EGESHARE_INCLUDE_CODE})
INCLUDE_DIRECTORIES(${EGESHARE_SOURCE_CODE})

SET(EGEJPEG_INCLUDE_CODE 		../../../libssrc/others/jpeg/)
SET(EGEJPEG_SOURCE_CODE 		../../../libssrc/others/jpeg/)
INCLUDE_DIRECTORIES(${EGEJPEG_INCLUDE_CODE})
INCLUDE_DIRECTORIES(${EGEJPEG_SOURCE_CODE})

SET(EGEFREETYPE_INCLUDE_CODE 		../../../libssrc/others/freetype2/include/)
SET(EGEFREETYPE_SOURCE_CODE 		../../../libssrc/others/freetype2/include/)
INCLUDE_DIRECTORIES(${EGEFREETYPE_INCLUDE_CODE})
INCLUDE_DIRECTORIES(${EGEFREETYPE_SOURCE_CODE})

#project file list
SET(${PROJECT_NAME}_sources
	main.cpp
)

#EMSCRIPTEN_SIM rappresenta ambiente simulato, ossia compilazione in linux (EMSCRIPTEN_SIM è un termine improprio)
if(EGE_DESTINATION_PLATFORM_STR STREQUAL "EMSCRIPTEN_SIM")
	#per emscripten non ci devono essere gli include perchè cosi usa i suoi precompilati internamente
    	# (se incluse le libs, il linker genera unresolved symbol)
       
	ADD_DEFINITIONS("-DEGE_EMSCRIPTEN_sim -DOPENGL_ES -DEGE_USE_WebGL2 -DBOOST_SYSTEM_NO_DEPRECATED")		
	set(EGE_SYS_LINUX "TRUE")
	set(CMAKE_C_FLAGS "-g")						# -std=c++17 -w -Wall -Wno-c++17-extensions
	set(CMAKE_CXX_FLAGS "-std=c++11 -g -w -v")			# -std=c++17 -w -Wall -Wno-c++17-extensions
	SET(EGE_EMSCRIPTEN_COMPILE_FLAGS "")				# -o0 -v
	SET(EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS  ${EGE_EMSCRIPTEN_COMPILE_FLAGS} )

    SET(EGELIBS ${EGEMAIN_PATH}/../../../build/lib/unix/)
	SET(PROJECT_NAME_EXT "${PROJECT_NAME}")
	ADD_EXECUTABLE( ${PROJECT_NAME_EXT} ${${PROJECT_NAME}_sources} )
    
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} GL GLEW glfw -L${EGELIBS} -lege2d -legejpeg -legeshare)
        
endif(EGE_DESTINATION_PLATFORM_STR STREQUAL "EMSCRIPTEN_SIM")


if(EGE_DESTINATION_PLATFORM_STR STREQUAL "EMSCRIPTEN")
	ADD_DEFINITIONS("-DEGE_EMSCRIPTEN -DOPENGL_ES -DEGE_USE_WebGL2 -DBOOST_SYSTEM_NO_DEPRECATED")	
    	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/linux_emscripten)

	set(EGE_SYS_EMSCRIPTEN "TRUE")

        #2018.02.22 - Aggiungo le dirs che mancano dalla compilazione di default (non capisco come mai dal momento che linux compila...)
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libssrc/others)		# percorso per X11/Xcursor/*.h 

	#Tutti i programmi e librerie vengono compilati in .bc (LLVM bytecode)
	#Tramite script si effettua il link verso un singolo HTML.
	#NOTA : L'opzione di compilazione -Ox DEVE essere uguale per tutte le librerie ed anche per il link esguito tramite script  <__EmccMakeHtml.sh>
	
	#SET(CMAKE_C_COMPILER /home/giovanni/emsdk-portable/emscripten/1.37.34/emcc)
	#SET(CMAKE_CXX_COMPILER /home/giovanni/emsdk-portable/emscripten/1.37.34/em++)
#	SET(EGE_EMSCRIPTEN_COMPILE_FLAGS "--bind -Oz -g4 --llvm-opts 0 -s NO_EXIT_RUNTIME=1 -s DEMANGLE_SUPPORT=0 -s USE_GLFW=3 -s LEGACY_GL_EMULATION=0 -s FULL_ES2=1 -s FULL_ES3=0 -lglfw3 -lGLEW" )
#	SET(EGE_EMSCRIPTEN_COMPILE_FLAGS "--bind -Oz -g0 -s NO_EXIT_RUNTIME=1 -s DEMANGLE_SUPPORT=0 -s USE_GLFW=3 -s LEGACY_GL_EMULATION=0 -s FULL_ES2=1 -s FULL_ES3=0 -lglfw3 -lGLEW" )
    SET(EGE_EMSCRIPTEN_COMPILE_FLAGS "--bind -Oz -g4 -s NO_EXIT_RUNTIME=1 -s DEMANGLE_SUPPORT=0 -s USE_GLFW=3 -s LEGACY_GL_EMULATION=0 -s FULL_ES2=1 -s FULL_ES3=0 -lglfw3 -lGLEW" )
	SET(EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS  ${EGE_EMSCRIPTEN_COMPILE_FLAGS} )

    set (CMAKE_C_FLAGS ${EGE_EMSCRIPTEN_COMPILE_FLAGS})
	set (CMAKE_CXX_FLAGS "${EGE_EMSCRIPTEN_COMPILE_FLAGS}")

endif(EGE_DESTINATION_PLATFORM_STR STREQUAL "EMSCRIPTEN")

#----------------------------------
#----------------------------------
#----------------------------------

if(EGE_DESTINATION_PLATFORM_STR STREQUAL "WIN32")
	set(EGE_SYS_WIN32 "TRUE")
	ADD_DEFINITIONS(-DWIN32) 		#variabile disponibile nel progetto con #ifdef
endif(EGE_DESTINATION_PLATFORM_STR STREQUAL "WIN32")

if(EGE_DESTINATION_PLATFORM_STR STREQUAL "WIN64")
	set(EGE_SYS_WIN64 "TRUE")
	ADD_DEFINITIONS(-DWIN64) 		#variabile disponibile nel progetto con #ifdef
endif(EGE_DESTINATION_PLATFORM_STR STREQUAL "WIN64")

if(EGE_DESTINATION_PLATFORM_STR STREQUAL "APPLEOSX")
	set(EGE_SYS_APPLE "TRUE")
	ADD_DEFINITIONS(-D__APPLE__) 		#variabile disponibile nel progetto con #ifdef
endif(EGE_DESTINATION_PLATFORM_STR STREQUAL "APPLEOSX")


#gimaf - termine preimpostazione

# set env
#----------------------------------
#if(UNIX)
#	ADD_DEFINITIONS(-Dlinux) 		#variabile disponibile nel progetto con #ifdef
#endif()
#if(WIN32)
#	ADD_DEFINITIONS(-DWIN32) 		#variabile disponibile nel progetto con #ifdef
#endif()
#if(APPLE)
#	ADD_DEFINITIONS(-D__APPLE__) 		#variabile disponibile nel progetto con #ifdef
	#TARGET_OS_IPHONE | TARGET_IPHONE_SIMULATOR    #verificare !!!
#endif()

#gimaf - imposta opzioni di compilazione progetti -----------------------------------

#set(EGE_EXES_GLOBAL_COMPILE_FLAGS "${EGE_EXES_GLOBAL_COMPILE_FLAGS} -std=c++17 -Wall -g -o0 -v")
set(EGE_EXES_GLOBAL_COMPILE_FLAGS "${EGE_EXES_GLOBAL_COMPILE_FLAGS} -std=c++17 -g -o0 ")
#set(EGE_STATICLIBS_GLOBAL_COMPILE_FLAGS "${EGE_STATICLIBS_GLOBAL_COMPILE_FLAGS} -std=c++17 -Wall -g -o0 -v")
set(EGE_STATICLIBS_GLOBAL_COMPILE_FLAGS "${EGE_STATICLIBS_GLOBAL_COMPILE_FLAGS} -std=c++17 -W0 -g -o0 -v")
    
set(EGE_SHAREDLIBS_GLOBAL_COMPILE_FLAGS "${EGE_SHAREDLIBS_GLOBAL_COMPILE_FLAGS} -std=c++17 -W0 -g -o0 -v")
# PS: Nel caso una LIB sovrascrive le impostazioni ad eccezione del fatto che venga impostato
#	il FORCE - se FORCE attivo allora tutte le dipendenze DEVONO essere compilate
#	con le opzioni GLOBAL_COMPILE_FLAG

#-std=c++17

set(EGE_GLOBAL_SETTING_FORCED "TRUE")
#------------------------------------------------------------------------------------

message ("******** ******** ******** ******** ******** ******** ******** ******** ******** ******** ******** ********* ")
if(EGE_SYS_LINUX)
message ("******** ******** ******** ***** OS ospite : è LINUX (impostato folders unix) ********** ********** ******** ") 
	LINK_DIRECTORIES( ${CMAKE_BINARY_DIR}/build)

	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib/unix)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib/unix)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin/unix)
endif()

if(EGE_SYS_EMSCRIPTEN)
message ("******** ******** ******** ***** OS ospite : è LINUX (impostato folders unix) ********** ********** ******** ") 
	LINK_DIRECTORIES( ${CMAKE_BINARY_DIR}/build)

	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib/emscripten)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib/emscripten)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin/emscripten)

	set(EGE_EMSCRIPTEN_JAVASCRIPT_LIBS_PATH "${PROJECT_SOURCE_DIR}/libssrc/others/emscripten-libs-js")
endif()

if(WIN32)
message ("******** ******** ******** ***** OS ospite : è WINDOWS (impostato folders windows) ******* ******** ******** ") 
message ("*** Progetto in cartella locale : ${PROJECT_BINARY_DIR}")
	LINK_DIRECTORIES( ${CMAKE_BINARY_DIR}/win32/libs )

	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/win32/dlls)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/win32/libs)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/win32/exe)
endif()

message("*** Direcories CMAKE_ARCHIVE_OUTPUT_DIRECTORY >" ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} "< ***")
message("*** Direcories CMAKE_LIBRARY_OUTPUT_DIRECTORY >" ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} "< ***")
message("*** Direcories CMAKE_RUNTIME_OUTPUT_DIRECTORY >" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} "< ***")

message ("***                                                                                                      *** ")
message ("******** ******** ******** ******** ******** ******** ******** ******** ******** ******** ******** ********* ")


