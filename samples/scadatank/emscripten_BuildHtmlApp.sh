# This script is designed to assemble the final html application into
# <yourprojectfolder>/build/bin/emscripten/ where are located *.bc library 
# common files after compilation.

# After "install.sh" (with emscripten toolchain selected and compilation 
# properly accomplished) we move also executable.bc(application) 
# and /resources/ in order to have  all relevant files in one folder 
# and where we can call __EmccMakeHtml.sh that's assembles all the 
# objects(.bc and resources) into a production .js file and .html that 
# can be called from a browser


echo Copy files to build directory

# copy resources
rm -Rf ./../../build/bin/emscripten/resources
mkdir ./../../build/bin/emscripten/resources
cp -r ./resources/. ./../../build/bin/emscripten/resources/.

# copy application file (executable.bc)
cp ./executable.bc ./../../build/bin/emscripten/executable.bc

# copy emscripten common use files
cp -r ./../../emscripten/. ./../../build/bin/emscripten/.

echo "Call batch file for compilation"
cd ./../../build/bin/emscripten
. __EmccMakeHtml.sh





