# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Linux)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER clang)
SET(CMAKE_CXX_COMPILER clang++)

SET (CMAKE_C_FLAGS "-g -w -v")					#-std=c++17 -w -Wall -Wno-c++17-extensions
SET (CMAKE_CXX_FLAGS "-std=c++17 -g -w -v")			# -std=c++17 -w -Wall -Wno-c++17-extensions

set (EGE_DESTINATION_PLATFORM_STR "EMSCRIPTEN_SIM")	#simula emscripten in eseguibile di linux

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
