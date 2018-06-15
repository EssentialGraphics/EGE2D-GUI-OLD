#include <egeConfig.h>

#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost;


// path with filename
std::string getexepath()
{
  char result[ PATH_MAX ];
  ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
  return std::string( result, (count > 0) ? count : 0 );
}

// path only without filename(executable)
std::string getpathonly(){
    string path=getexepath();
    
    int pos=path.find_last_of("/");
    
    return path.substr(0,pos);
}
