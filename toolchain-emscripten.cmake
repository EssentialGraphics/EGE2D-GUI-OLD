# the name of the target operating system
SET(CMAKE_SYSTEM_NAME EMSCRIPTEN)

# which compilers to use for C and C++

SET(CMAKE_C_COMPILER emcc)
SET(CMAKE_CXX_COMPILER em++)

set (EGE_DESTINATION_PLATFORM_STR "EMSCRIPTEN")	#compila n. files .bc , che devono essere poi linkati tramite script .sh in /bin/mscripten

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
