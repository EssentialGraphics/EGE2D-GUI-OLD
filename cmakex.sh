echo  ----  CMake toolchain    -----
echo 
echo cache files are deleted for safety
rm CMakeCache.txt
rm cmake_install.cmake
rm -Rf CMakeFiles

# Uncomment just one of the sentence below in order to select toolchain 

#cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-clang.cmake .
#cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-gnu.cmake .
#cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-emscripten.cmake .
cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-win64.cmake .

