echo  ----  CMake toolchain    -----
echo 
echo cache files are deleted for safety
rm CMakeCache.txt
rm cmake_install.cmake
rm -Rf CMakeFiles

# Uncomment just one of the sentence below in order to select toolchain 

cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-clang.cmake .             # linux CLang c++ compiler
#cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-gnu.cmake .               # linux GNU c++ compiler
#cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-emscripten.cmake .         # javascript code (asm.js)
#cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-win64.cmake .             # windows libs and exe

