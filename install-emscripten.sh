echo "Downloads EMSCRIPTEN."

cd ..

# Get the emsdk repo
git clone https://github.com/juj/emsdk.git

# Enter that directory
cd emsdk

# Fetch the latest registry of available tools.
./emsdk update

# Download and install the latest SDK tools.
./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes ~/.emscripten file)
./emsdk activate latest

# Activate PATH and other environment variables in the current terminal
source ./emsdk_env.sh

echo " IMPORTANT TO DO !!!! --------------------------------------------------------------------------------------"
echo " Open the file ./emsdk/emsdk_set_env.sh and copy its content into your .bashrc file in your home folder"
echo " This is used to supply the proper path to EMSCRIPTEN"
echo " -----------------------------------------------------------------------------------------------------------"
