#2016 Sett. 1
# Il progetto EMCC è composto da N. librerie (.bc) compilate tramite CMake.
# E' necessario LINKARLE assieme per generare il progetto finale.
# NOTA: Le libs che compongono il progetto DEVONO NECESSARIAMENTE avere lo stesso livello di ottimizzazione (-O2) altrimenti
# emcc non converte correttamente le chiamate tra di esse. (E' una specifica EMCC)  

#ESEMPIO FUNZIONANTE DA EMSCRIPTEN OpenGLBook samples
#emcc -s LEGACY_GL_EMULATION=1 -O2 -s USE_GLFW=3 main.cpp esUtil.c -o /var/www/html/emscripten/main.html

#Compilato e Funzionante su server Apache !!
#emcc -std=c++11 -O2 -s LEGACY_GL_EMULATION=1 -s USE_GLFW=3 ConvertUTF.bc jpeg.bc zlib.bc assimp.bc OpenGLBasic.bc -o OpenGLBasic.html

#------------------------------------------------------------------------------------------------------------------------------
#NOTA : errore in compilazione tipo : --->>>>   "warning: unresolved symbol: glBindFramebufferEXT" - 
#	1) Aggiunto -lGLEW alla compilazione ma dovrebbe essere già incluso in emcc di default
#	2) Alcune funzioni di GLEW non sono implementate in emcc, allora le ho sostituite con altre "riconosciute" da emcc
#	3) Per le librerie mie dovrebbero essere compilate in EMCC per avere riferimenti al linker
#------------------------------------------------------------------------------------------------------------------------------

echo Compilazione in corso EMSCRIPTEN...

#--shell-file path/to/custom_shell.html

EMCC_DEBUG=1 emcc  -s "EXPORTED_FUNCTIONS=['_main','_onComm','_onMessageAsString']" -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap']" -v -Oz -g4 -Werror  -s DEMANGLE_SUPPORT=1 -s TOTAL_MEMORY=33554432  -s SAFE_HEAP=1 -s ALIASING_FUNCTION_POINTERS=0 -s ASSERTIONS=2  -s GL_UNSAFE_OPTS=0 -s DISABLE_EXCEPTION_CATCHING=0 -s USE_GLFW=3 -s GL_FFP_ONLY=1 -s USE_WEBGL2=0 -s LEGACY_GL_EMULATION=0 -s FULL_ES2=1 -s FULL_ES3=0 --js-opts 1 --js-library library_websockets.js egejpeg.bc egefreetype.bc egeshare.bc ege2d.bc executable.bc -o ege.html --preload-file resources/ --shell-file shell.html


# -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']"
# -s EXPORTED_FUNCTIONS='["_onOpenCallback"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'
#o una o l'altra
# -s LEGACY_GL_EMULATION=1
# -s FULL_ES2=1 

# impostazione shell custom
#--shell-file shell_ege.html

#echo Copia files in /var/www/localhost/htdocs/emscripten/
#cp ege.js /var/www/localhost/htdocs/emscripten/
#cp ege.html /var/www/localhost/htdocs/emscripten
#cp ege.data /var/www/localhost/htdocs/emscripten
#cp ege.html.mem /var/www/localhost/htdocs/emscripten

#-s SAFE_HEAP=1
#-s GL_UNSAFE_OPTS=2
#-s FORCE_ALIGNED_MEMORY=1
