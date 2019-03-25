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
#include <iomanip> // std::setfill, std::setw
#include <map>
// <php>
namespace php
{
std::string file_get_contents(const std::string &$filename)
{
    std::ostringstream ss;
    ss << std::ifstream($filename).rdbuf();
    return ss.str();
}

// php's rtrim() in c++
std::string rtrim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6))
{
    // optimizeme: can this be optimized to a single erase() call? probably.
    while ($str.size() > 0 && $character_mask.find_first_of($str.back()) != std::string::npos)
    {
        $str.pop_back();
    }
    return $str;
}
// php's ltrim() in c++
std::string ltrim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6))
{
    // optimizeme: can this be optimized to a single erase() call? probably.
    while ($str.size() > 0 && $character_mask.find_first_of($str.front()) != std::string::npos)
    {
        $str.erase(0, 1);
    }
    return $str;
}
// php's trim() in c++
std::string trim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6))
{
    return rtrim(ltrim($str, $character_mask), $character_mask);
}
// php's explode in c++
std::vector<std::string> explode(const std::string &$delimiter, const std::string &$string, const size_t $limit = std::numeric_limits<size_t>::max())
{
    if ($delimiter.empty())
    {
        throw std::invalid_argument("delimiter cannot be empty!");
    }
    std::vector<std::string> ret;
    if ($limit <= 0)
    {
        return ret;
    }
    size_t pos = 0;
    size_t next_pos = $string.find($delimiter);
    if (next_pos == std::string::npos || $limit == 1)
    {
        ret.push_back($string);
        return ret;
    }
    for (;;)
    {
        ret.push_back($string.substr(pos, next_pos - pos));
        pos = next_pos + $delimiter.size();
        if (ret.size() >= ($limit - 1) || std::string::npos == (next_pos = $string.find($delimiter, pos)))
        {
            ret.push_back($string.substr(pos));
            return ret;
        }
    }
}
// php's implode() in c++
std::string implode(const std::string &$glue, const std::vector<std::string> &$pieces)
{
    std::string ret;
    for (size_t i = 0; i < $pieces.size(); ++i)
    {
        ret.append($pieces[i]);
        if (i + 1 < $pieces.size())
        {
            ret.append($glue);
        }
    }
    return ret;
}
bool file_exists(const std::string &$filename)
{
    // this is bugged, as the file has to "exist AND be readable",
    // but in c++17 and not bugged:
    //    return std::filesystem::exists($filename);
    std::ifstream f($filename.c_str());
    return f.good();
}
std::string str_replace(const std::string &$search, const std::string &$replace, std::string $subject, size_t &$count /*=0*/)
{
    $count = 0;
    size_t pos = 0, newpos;
    while (std::string::npos != (newpos = $subject.find($search, pos)))
    {
        $subject.replace(newpos, $search.size(), $replace);
        ++$count;
        pos = newpos + $replace.size();
    }
    return $subject;
}
// overload to make $count optional
std::string str_replace(const std::string &$search, const std::string &$replace, std::string $subject)
{
    size_t dummy_count;
    return str_replace($search, $replace, $subject, dummy_count);
}
std::string getcwd(void)
{
    // with c++17: std::filesystem::current_path
    // FIXME PATH_MAX blahblah
    std::string ret(0xFFFF, '\00');
    ::getcwd(&ret[0], ret.size());
    ret.erase(ret.find('\00'));
    ret.shrink_to_fit();
    return ret;
}
std::string strtolower(std::string $string)
{
    std::transform($string.begin(), $string.end(), $string.begin(), ::tolower);
    return $string;
}
std::string strtoupper(std::string $string)
{
    std::transform($string.begin(), $string.end(), $string.begin(), ::toupper);
    return $string;
}
std::string bin2hex(const std::string &$str)
{
    // from https://stackoverflow.com/a/18906469/1067003
    std::string ossbuf;
    ossbuf.reserve($str.size() * 2);
    std::ostringstream out(std::move(ossbuf));
    out << std::hex << std::setfill('0');
    for (unsigned char c : $str)
    {
        out << std::setw(2) << uint_fast16_t(c);
    }
    return out.str();
}
std::string hex2bin(const std::string &$str)
{
    // from https://stackoverflow.com/a/18906469/1067003
    std::string ret;
    if ($str.empty())
    {
        return ret;
    }
    if (($str.size() % 2) != 0)
    {
        throw std::invalid_argument("Hexadecimal input string must have an even length");
    }
    ret.reserve(size_t($str.size() / 2));
    for (size_t i = 0; i < $str.length(); i += 2)
    {
        uint_fast16_t tmp;
        if (std::istringstream($str.substr(i, 2)) >> std::hex >> tmp)
        {
            ret.push_back(tmp);
        }
        else
        {
            throw std::invalid_argument("at offset " + std::to_string(i == 0 ? 0 : (i / 2)) + ": invalid hex");
        }
    }
    return ret;
}

std::string strtr(std::string $str, const std::string &$from, const std::string &$to)
{
    if ($from.size() != $to.size())
    {
        throw std::invalid_argument("from size and to size must be equal!");
    }
    size_t pos = 0;
    for (;;)
    {
        pos = $str.find_first_of($from, pos);
        if (pos == std::string::npos)
        {
            return $str;
        }
        $str[pos] = $to[$from.find_first_of($str[pos])];
        ++pos;
    }
    return $str;
}
std::string strtr(const std::string &$str, const std::map<std::string, std::string> &$replace_pairs)
{
    //this function is not optimized, a much faster implementation is probably possible to make.
    std::vector<std::string> keys_sorted;
    keys_sorted.reserve($replace_pairs.size());
    for (auto it = $replace_pairs.begin(); it != $replace_pairs.end(); ++it)
    {
        keys_sorted.push_back(it->first);
    }
    std::sort(keys_sorted.begin(), keys_sorted.end(), [](const std::string &s1, const std::string &s2) -> bool { return s1.size() > s2.size(); });
#ifdef DEBUG_STRTR
    for (const auto &key : keys_sorted)
    {
        std::cout << "\"" << key << "\" => \"" << $replace_pairs.at(key) << "\"" << std::endl;
    }
#endif
    std::string ret;
    for (size_t i = 0; i < $str.size();)
    {
        bool found = false;
        for (const auto &key : keys_sorted)
        {
            if (i == $str.find(key, i))
            {
                ret.append($replace_pairs.at(key));
                i += key.size(); // or should it be key.size() + 1 ?
                found = true;
                break;
            }
            else
            {
#ifdef DEBUG_STRTR
                std::cout << "apparently \"" << key << "\" is not the same as \"" << $str[i] << "\"" << std::endl;
#endif
            }
        }
        if (!found)
        {
            ret.push_back($str[i]);
            ++i;
        }
    }
    return ret;
}

} // namespace php
