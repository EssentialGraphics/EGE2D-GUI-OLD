#This is a helper installer to be run first
# IMPORTANT : remember to call this script with the form ". install.sh" in order to leave the scripts to change directory.
# 	      If "." is missing then <cd> script commands aren't executed

echo "################################### MAIN INSTALLER ############################################################"
echo "#"
echo "# PRE-REQUISITES to be met before to run this script:"
echo "#"
echo "# Install all dependencies for your system as described in relevant file"
echo "# <EGE2D-*******-dependencies list.txt> located in this project root directory."
echo "# (sudo apt-get / yum / dnf / emerge etc..)"
echo "#"
echo "# Execute manually the following commands in order to compile with enough thrustee"
echo "# chmod +rwx install.sh"
echo "# chmod +rwx cmakex.sh"
echo "# chmod +rwx cleanall.sh"
echo "#"
echo "# Remember to edit cmakex.sh in order to select your favorite destination framework OS via toolchain"
echo "# (Default is Linux) - available are GNU-Linux-C++ / CLang-Linux-C++ / Emscripten(asm.js)."
echo "#"
echo "##############################################################################################################"
echo
read -n1 -r -p "Press any key if you want to continue... , CTRL+C to abort" key

echo
echo 1. Automatically downloads git libs from web
cd libssrc/others
pwd
git clone http://git.sv.nongnu.org/r/freetype/freetype2.git
echo "Downloads exactly versione 2.9 for freetype. Due to compatibility reasons."
rm -Rf freetype2
wget "http://download.savannah.gnu.org/releases/freetype/freetype-2.9.tar.bz2"
tar -xjvf freetype-2.9.tar.bz2
mv freetype-2.9 freetype2
echo
echo "Downloads EMSCRIPTEN."
##   #OLD #git clone http://github.com/kripken/emscripten.git
#git clone https://github.com/juj/emsdk.git
#cd emsdk
# Fetch the latest registry of available tools.
#./emsdk update
# Download and install the latest SDK tools.
#./emsdk install latest
# Make the "latest" SDK "active" for the current user. (writes ~/.emscripten file)
#./emsdk activate latest
# Activate PATH and other environment variables in the current terminal
#source ./emsdk_env.sh
#cd ..
echo
echo "Downloads BOOST library"
wget -c "http://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.tar.bz2/download"
tar xf download
echo
echo "Downloads GLFW 3.2.1"
git clone https://github.com/glfw/glfw.git
echo
cd ..
cd ..
echo 1.1 Update CMakeLists.txt with our custom version
cp ./libssrc/others/freetype2-NewCMakeLists/CMakeLists.txt ./libssrc/others/freetype2/CMakeLists.txt

echo
echo 2. Runs cmakex.sh 
echo 	.default destination is gnu-linux
echo 	.edit "cmakex.sh" if you want to switch toolchain "(i.e for emscripten(.js)/clang/microsoft(win)/etc..)"
echo
./cmakex.sh


echo
echo 3. Libraries compilation "(make)"
make


echo
echo 4. Compiling samples and tutorials
echo 	4.1 Tutorial 01
cd samples/tutorials/tutorial-01
cp ./../../../cmakex.sh cmakex.sh
cp ./../../../toolchain-gnu.cmake toolchain-gnu.cmake
cp ./../../../toolchain-clang.cmake toolchain-clang.cmake
cp ./../../../toolchain-emscripten.cmake toolchain-emscripten.cmake
./cmakex.sh
make
cd ..
cd ..
cd ..

echo 	4.2 Tutorial 02
cd samples/tutorials/tutorial-02
cp ./../../../cmakex.sh cmakex.sh
cp ./../../../toolchain-gnu.cmake toolchain-gnu.cmake
cp ./../../../toolchain-clang.cmake toolchain-clang.cmake
cp ./../../../toolchain-emscripten.cmake toolchain-emscripten.cmake
./cmakex.sh
make
cd ..
cd ..
cd ..

echo 	4.3 Tutorial 03
cd samples/tutorials/tutorial-03
cp ./../../../cmakex.sh cmakex.sh
cp ./../../../toolchain-gnu.cmake toolchain-gnu.cmake
cp ./../../../toolchain-clang.cmake toolchain-clang.cmake
cp ./../../../toolchain-emscripten.cmake toolchain-emscripten.cmake
./cmakex.sh
make
cd ..
cd ..
cd ..


echo 	5 Scada
cd samples/scadatank
cp ./../../cmakex.sh cmakex.sh
cp ./../../toolchain-gnu.cmake toolchain-gnu.cmake
cp ./../../toolchain-clang.cmake toolchain-clang.cmake
cp ./../../toolchain-emscripten.cmake toolchain-emscripten.cmake
./cmakex.sh
make
cd ..
cd ..

echo
echo END OF SCRIPT
echo
