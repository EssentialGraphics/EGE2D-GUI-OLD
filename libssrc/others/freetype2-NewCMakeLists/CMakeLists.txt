cmake_minimum_required(VERSION 3.4)

#questo cmake rimpiazza l'originale per compilazione anche con emscripten

include(CheckIncludeFile)

#LIBRARY PROJECT
#----------------------------------
PROJECT(egefreetype)

set(VERSION_MAJOR "2")
set(VERSION_MINOR "9")
set(VERSION_PATCH "0")

set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(SHARED_LIBRARY_VERSION ${VERSION_MAJOR}.${VERSION_MINOR})

# Compiler definitions for building the library
add_definitions(-DFT2_BUILD_LIBRARY)
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

message ("******** ******** ******** ******** ******** ******** ******** ******** ******** ******** ******** ********* ")
message ("******** 						FREETYPE LIBRARY 	                     ********* ")
message ("******** ******** ******** ******** ******** ******** ******** ******** ******** ******** ******** ********* ")

# Create the configuration file
message(STATUS
  "Creating file ${PROJECT_BINARY_DIR}/include/freetype/config/ftconfig.h")

message(STATUS
  "Creating directory ${PROJECT_BINARY_DIR}/include/freetype/config")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/include/freetype/config")


message ("  >>> >>> >>> >>> >>> 	Cerco librerie disponibili per la compilazione 		<<< <<< <<< <<< <<<")
# Find dependencies
foreach (d ZLIB BZip2 PNG HarfBuzz)
  string(TOUPPER "${d}" D)

  if (DEFINED WITH_${d} OR DEFINED WITH_${D})
    if (WITH_${d} OR WITH_${D})
      find_package(${d} QUIET REQUIRED)
    endif ()
  else ()
    find_package(${d} QUIET)
  endif ()

  if (${d}_FOUND OR ${D}_FOUND)
    message(STATUS "Building with ${d}")
  endif ()
endforeach ()
message ("  >>> >>> >>> >>> >>> 	---------------------------------------------- 		<<< <<< <<< <<< <<<")


if (UNIX)

  message ("******** 					E' attivo flag UNIX per freetype 			       ********* ")
  
  check_include_file("unistd.h" HAVE_UNISTD_H)
  check_include_file("fcntl.h" HAVE_FCNTL_H)
  check_include_file("stdint.h" HAVE_STDINT_H)

  file(READ "${PROJECT_SOURCE_DIR}/builds/unix/ftconfig.in"
    FTCONFIG_H)
  if (HAVE_UNISTD_H)
    string(REGEX REPLACE
      "#undef +(HAVE_UNISTD_H)" "#define \\1"
      FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  if (HAVE_FCNTL_H)
    string(REGEX REPLACE
      "#undef +(HAVE_FCNTL_H)" "#define \\1"
      FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  if (HAVE_STDINT_H)
    string(REGEX REPLACE
      "#undef +(HAVE_STDINT_H)" "#define \\1"
      FTCONFIG_H "${FTCONFIG_H}")
  endif ()
  string(REPLACE "/undef " "#undef "
    FTCONFIG_H "${FTCONFIG_H}")
  file(WRITE "${PROJECT_BINARY_DIR}/include/freetype/config/ftconfig.h-new"
    "${FTCONFIG_H}")
else ()
  file(READ "${PROJECT_SOURCE_DIR}/include/freetype/config/ftconfig.h"
    FTCONFIG_H)
  file(WRITE "${PROJECT_BINARY_DIR}/include/freetype/config/ftconfig.h-new"
    "${FTCONFIG_H}")
endif ()

#execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
#  "${PROJECT_BINARY_DIR}/include/freetype/config/ftconfig.h-new"
#  "${PROJECT_BINARY_DIR}/include/freetype/config/ftconfig.h")

SET(${PROJECT_NAME}_headers 

)

set(${PROJECT_NAME}_sources
	  src/autofit/autofit.c
  src/base/ftbase.c
  src/base/ftbbox.c
  src/base/ftbdf.c
  src/base/ftbitmap.c
  src/base/ftcid.c
  src/base/ftfntfmt.c
  src/base/ftfstype.c
  src/base/ftgasp.c
  src/base/ftglyph.c
  src/base/ftgxval.c
  src/base/ftinit.c
  src/base/ftlcdfil.c
  src/base/ftmm.c
  src/base/ftotval.c
  src/base/ftpatent.c
  src/base/ftpfr.c
  src/base/ftstroke.c
  src/base/ftsynth.c
  src/base/ftsystem.c
  src/base/fttype1.c
  src/base/ftwinfnt.c
  src/bdf/bdf.c
  src/bzip2/ftbzip2.c
  src/cache/ftcache.c
  src/cff/cff.c
  src/cid/type1cid.c
  src/gzip/ftgzip.c
  src/lzw/ftlzw.c
  src/pcf/pcf.c
  src/pfr/pfr.c
  src/psaux/psaux.c
  src/pshinter/pshinter.c
  src/psnames/psnames.c
  src/raster/raster.c
  src/sfnt/sfnt.c
  src/smooth/smooth.c
  src/truetype/truetype.c
  src/type1/type1.c
  src/type42/type42.c
  src/winfonts/winfnt.c

)

message ("${PROJECT_NAME}:> Linker using ${EGE_DESTINATION_PLATFORM_STR} setting for project : <<${PROJECT_NAME}>> ***")
# create the library
#----------------------------------
if(EGE_SYS_EMSCRIPTEN)
	SET(PROJECT_NAME_EXT ${PROJECT_NAME}.bc)
	message("${PROJECT_NAME}:> Libreria STATICA .bc (da linkare al programma) ***")
	
	ADD_EXECUTABLE(${PROJECT_NAME_EXT} ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})
	message("${PROJECT_NAME}:> ESEGUIBILE in : <<${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>> ***")

	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden" )
	#set(CMAKE_CXX_FLAGS "${EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS}" )
	#SET_TARGET_PROPERTIES(${PROJECT_NAME_EXT} PROPERTIES COMPILE_FLAGS "${EGE_EMSCRIPTEN_STATIC_LIBS_COMPILE_FLAGS} " )

	message("${PROJECT_NAME}:> ESEGUIBILE in : <<${CMAKE_RUNTIME_OUTPUT_DIRECTORY}>>")


endif()

if(EGE_SYS_LINUX)
	SET(PROJECT_NAME_EXT ${PROJECT_NAME})
	message ("${PROJECT_NAME}:> Libreria STATICA .a (da linkare al programma) ***")
	
	ADD_LIBRARY(${PROJECT_NAME_EXT} STATIC ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})
	message("${PROJECT_NAME}:> LIBRERIA in : <<${CMAKE_LIBRARY_OUTPUT_DIRECTORY}>> ***")
endif()



# linker
#----------------------------------
#TARGET_LINK_LIBRARIES(${PROJECT_NAME}) 
message("${PROJECT_NAME}:> Cartella progetto: ${CMAKE_CURRENT_SOURCE_DIR}")
INSTALL( 	
		TARGETS ${PROJECT_NAME_EXT}
		RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
		ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}     
)



