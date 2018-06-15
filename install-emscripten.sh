echo "Downloads EMSCRIPTEN."

cd ..
#git clone https://github.com/juj/emsdk.git
git clone http://github.com/kripken/emscripten.git
cd emsdk
# Fetch the latest registry of available tools.
./emsdk update
# Download and install the latest SDK tools.
# ./emsdk install latest
./emsdk install 1.38.6

# Make the "latest" SDK "active" for the current user. (writes ~/.emscripten file)
#./emsdk activate latest
./emsdk activate 1.38.6

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh
cd ..

EGE2D NOTE : it's usefull to copy and paste PATHs contained in just installed emsdk_env.sh to your .bashrc file to make emcc/em++ 
reachebles from any teminal window. thanks

