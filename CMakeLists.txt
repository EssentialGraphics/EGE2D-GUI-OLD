#Compilation process:

# edit cmakex.sh -> for the toolchain you prefer (clang/gnu/emscripten etc...)
# sh ./cmakex.sh -> from console 
# make  -> to compile

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#About this project
#----------------------------------
PROJECT(EssentialGraphicEngine)
SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "0")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

message(">>>> ----------------------------------------------------------------------- <<<<")
message(">>>> ESSENTIAL GRAPHIC ENGINE - SOURCE DIRECTORY : " ${PROJECT_BINARY_DIR} " <<<<")

SET(EGEMAIN_PATH 	${PROJECT_BINARY_DIR})
INCLUDE_DIRECTORIES(${EGEMAIN_PATH})

SET(EGE_INCLUDE_JPEG 		${EGEMAIN_PATH}/libssrc/others/jpeg/)
INCLUDE_DIRECTORIES(${EGE_INCLUDE_JPEG})		

SET(EGE_INCLUDE_FREETYPE2 	${EGEMAIN_PATH}/libssrc/others/freetype2/include/)
INCLUDE_DIRECTORIES(${EGE_INCLUDE_FREETYPE2})		

SET(EGESHARE_SOURCE_CODE 	${EGEMAIN_PATH}/libssrc/egeshare/src/)
INCLUDE_DIRECTORIES(${EGESHARE_SOURCE_CODE})

SET(EGE2D_INCLUDE_CODE 		${EGEMAIN_PATH}/libssrc/ege2d/)
SET(EGE2D_SOURCE_CODE 		${EGEMAIN_PATH}/libssrc/ege2d/)
INCLUDE_DIRECTORIES(${EGE2D_INCLUDE_CODE})
INCLUDE_DIRECTORIES(${EGE2D_SOURCE_CODE})

SET(EGEBOOST_CODE 		${EGEMAIN_PATH}/libssrc/others/boost_1_66_0/)
INCLUDE_DIRECTORIES(${EGEBOOST_CODE})

# we assume emscripten is installed through install-emscripten.sh and environment variable EMSCRIPTEN is set in .bashrc
SET(EGEEMSCRIPTEN_CODE 		$ENV{EMSCRIPTEN})
INCLUDE_DIRECTORIES(${EGEEMSCRIPTEN_CODE})

#informazioni di DEBUG
#ADD_DEFINITIONS("-D__ege_printinfo_textures_on")	#richiama funzione PRINTINFO() per visualizzare specchietto info texture carita (H/W/size etc...)
ADD_DEFINITIONS("-D__ege_printinfo_textures_off")	#richiama funzione PRINTINFO() per visualizzare specchietto info texture carita (H/W/size etc...)

#gimaf - Imposta tipo di compilazione (emscripten / Win / Unix / Android / OSX etc..)
#set (EGE_DESTINATION_PLATFORM_STR "WIN32")
#set (EGE_DESTINATION_PLATFORM_STR "OSX")
#set (EGE_DESTINATION_PLATFORM_STR "ANDROID")
#set (EGE_DESTINATION_PLATFORM_STR "LINUX")
#set (EGE_DESTINATION_PLATFORM_STR "EMSCRIPTEN")	#compila n. files .bc , che devono essere poi linkati tramite script .sh in /bin/mscripten
#set (EGE_DESTINATION_PLATFORM_STR "EMSCRIPTEN_SIM")	#simula emscripten in eseguibile di linux	
# PS : L'ultimo SET attiva la configurazione desiderata. 
# 2018 04 08 - ora l'impostazione avviene nei file TOOLCHAIN

#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libssrc/others/boost-1.66)
#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libssrc/others/freetype2/include)

message("> *** Compile for destination platform : >" ${EGE_DESTINATION_PLATFORM_STR} "< ***")

#EMSCRIPTEN_SIM rappresenta ambiente simulato, ossia compilazione in linux (EMSCRIPTEN_SIM è un termine improprio)
if(EGE_DESTINATION_PLATFORM_STR STREQUAL "EMSCRIPTEN_SIM")
	#per emscripten non ci devono essere gli include perchè cosi usa i suoi precompilati internamente
    	# (se incluse le libs, il linker genera unresolved symbol)
    	
        #INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libssrc/others/glew-2.0.0/include)
	#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libssrc/others/glfw-3.2.1/include)

	ADD_DEFINITIONS("-DEGE_EMSCRIPTEN_sim -DOPENGL_ES -DEGE_USE_WebGL2 -DBOOST_SYSTEM_NO_DEPRECATED")		
	INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/linux_opengl)
	set(EGE_SYS_LINUX "TRUE")
	set(CMAKE_C_FLAGS "-g")				#-std=c++17 -w -Wall -Wno-c++17-extensions
	set(CMAKE_CXX_FLAGS "-std=c++11 -g -w -v")			# -std=c++17 -w -Wall -Wno-c++17-extensions
	SET(EGE_EMSCRIPTEN_COMPILE_FLAGS "")				# -o0 -v
	SET(EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS  ${EGE_EMSCRIPTEN_COMPILE_FLAGS} )
endif(EGE_DESTINATION_PLATFORM_STR STREQUAL "EMSCRIPTEN_SIM")


if(EGE_DESTINATION_PLATFORM_STR STREQUAL "EMSCRIPTEN")
	ADD_DEFINITIONS("-DEGE_EMSCRIPTEN -DOPENGL_ES -DEGE_USE_WebGL2 -DBOOST_SYSTEM_NO_DEPRECATED")	
        
	set(EGE_SYS_EMSCRIPTEN "TRUE")
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
# add subfolders
#----------------------------------

ADD_SUBDIRECTORY(libssrc)
#ADD_SUBDIRECTORY(src)   #programma app?

message(" ----------------------------------------------------------------------------------------------------------------------------- ")
message(" 						ISTRUZIONI DI COMPILAZIONE")
message(" Procedura : Impostare piattaforma destinazione EGE_DESTINATION_PLATFORM_STR")
message(" Eseguire >cmake .")
message(" Eseguire >make")
message(" per EMSCRIPTEN : E' necessario eseguire LINK di tutti i moduli .bc creati da make dopo CMake tramite shell script <__EmccMakeHtml.sh> ")
message(" 		   così da ottenere un file .html contente tutto il codice programma e librerie. (Eseguire manualmente)")
message(" ----------------------------------------------------------------------------------------------------------------------------- ")

