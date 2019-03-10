// todo: trim includes - includes were extracted from another project and includes were not trimed
#include <iostream>
#include <string>
#include <fstream>
// when c++17 becomes available..
//#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <exception>
#include <limits>
#include <algorithm>
#include <unistd.h>
// <php>
namespace php
{
std::string file_get_contents(const std::string& $filename);
// php's rtrim() in c++
std::string rtrim ( std::string $str, const std::string& $character_mask = "\x20\x09\x0A\x0D\x00\x0B"  );
// php's ltrim() in c++
std::string ltrim ( std::string $str, const std::string& $character_mask = "\x20\x09\x0A\x0D\x00\x0B"  );
// php's trim() in c++
std::string trim(std::string $str, const std::string& $character_mask = "\x20\x09\x0A\x0D\x00\x0B");
// php's explode in c++
std::vector<std::string> explode(const std::string& $delimiter,const std::string& $string, const size_t $limit = std::numeric_limits<size_t>::max());
// php's implode() in c++
std::string implode ( const std::string& $glue, const std::vector<std::string>& $pieces );
bool file_exists ( const std::string& $filename );
std::string str_replace ( const std::string& $search , const std::string& $replace , std::string $subject, size_t &$count/*=0*/ );
// overload to make $count optional
std::string str_replace ( const std::string& $search, const std::string& $replace, std::string $subject);
std::string strtolower ( std::string $string );
std::string getcwd(void);

}//</php>

