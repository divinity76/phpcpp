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
std::string file_get_contents(const std::string& $filename)
{
    std::ostringstream ss;
    ss << std::ifstream($filename).rdbuf();
    return ss.str();
}

// php's rtrim() in c++
std::string rtrim ( std::string $str, const std::string& $character_mask = "\x20\x09\x0A\x0D\x00\x0B"  )
{
    // optimizeme: can this be optimized to a single erase() call? probably.
    while($str.size() > 0 && $character_mask.find_first_of($str.back()) != std::string::npos)
    {
        $str.pop_back();
    }
    return $str;
}
// php's ltrim() in c++
std::string ltrim ( std::string $str, const std::string& $character_mask = "\x20\x09\x0A\x0D\x00\x0B"  )
{
    // optimizeme: can this be optimized to a single erase() call? probably.
    while($str.size() > 0 && $character_mask.find_first_of($str.front()) != std::string::npos)
    {
        $str.erase(0,1);
    }
    return $str;
}
// php's trim() in c++
std::string trim(std::string $str, const std::string& $character_mask = "\x20\x09\x0A\x0D\x00\x0B")
{
    return rtrim(ltrim($str,$character_mask),$character_mask);
}
// php's explode in c++
std::vector<std::string> explode(const std::string& $delimiter,const std::string& $string, const size_t $limit = std::numeric_limits<size_t>::max())
{
    if($delimiter.empty())
    {
        throw std::invalid_argument("delimiter cannot be empty!");
    }
    std::vector<std::string> ret;
    if($limit <= 0)
    {
        return ret;
    }
    size_t pos = 0;
    size_t next_pos = $string.find($delimiter);
    if(next_pos == std::string::npos || $limit == 1)
    {
        ret.push_back($string);
        return ret;
    }
    for (;;)
    {
        ret.push_back($string.substr(pos,next_pos-pos));
        pos=next_pos+$delimiter.size();
        if(ret.size() >= ($limit-1) || std::string::npos == (next_pos=$string.find($delimiter,pos)))
        {
            ret.push_back($string.substr(pos));
            return ret;
        }
    }
}
// php's implode() in c++
std::string implode ( const string& $glue, const std::vector<std::string>& $pieces )
{
    std::string ret;
    for(size_t i=0; i < $pieces.size(); ++i)
    {
        ret.append($pieces[i]);
        if(i+1 < $pieces.size())
        {
            ret.append($glue);
        }
    }
    return ret;
}
bool file_exists ( const string& $filename ){
    // this is bugged, as the file has to "exist AND be readable",
    // but in c++17 and not bugged:
    //    return std::filesystem::exists($filename);
    ifstream f($filename.c_str());
    return f.good();
}
std::string str_replace ( const std::string& $search, const std::string& $replace, std::string $subject, size_t &$count/*=0*/)
{
    $count=0;
    size_t pos=0,newpos;
    while(std::string::npos != (newpos=$subject.find($search,pos)))
    {
        $subject.replace(newpos,$search.size(),$replace);
        pos=newpos+$search.size();
    }
    return $subject;
}
// overload to make $count optional
std::string str_replace ( const std::string& $search, const std::string& $replace, std::string $subject)
{
    size_t dummy_count;
    return str_replace($search,$replace,$subject,dummy_count);
}

}//</php>

