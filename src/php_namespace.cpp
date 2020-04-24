#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <iomanip> // std::setfill, std::setw
#include <map>
#if __cplusplus >= 201703L
#include <filesystem>
#endif
#include <chrono>
#include <cassert>
#include <random>
#include <cstddef>

// <php>
namespace php
{
std::string file_get_contents(const std::string &$filename)
{
    std::ifstream file($filename, std::ifstream::binary);
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.seekg(0, std::istream::end);
    const std::streampos ssize = file.tellg();
    if (ssize < 0)
    {
        // can't get size for some reason, fallback to slower "just read everything"
        // because i dont trust that we could seek back/fourth in the original stream,
        // im creating a new stream.
        std::ifstream file($filename, std::ifstream::binary);
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
    file.seekg(0, std::istream::beg);
    std::string result(ssize, 0);
    file.read(&result[0], ssize);
    return result;
}

std::string rtrim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6))
{
    // optimizeme: can this be optimized to a single erase() call? probably.
    while ($str.size() > 0 && $character_mask.find_first_of($str.back()) != std::string::npos)
    {
        $str.pop_back();
    }
    return $str;
}
std::string ltrim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6))
{
    // optimizeme: can this be optimized to a single erase() call? probably.
    while ($str.size() > 0 && $character_mask.find_first_of($str.front()) != std::string::npos)
    {
        $str.erase(0, 1);
    }
    return $str;
}
std::string trim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6))
{
    return rtrim(ltrim($str, $character_mask), $character_mask);
}
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
#if __cplusplus >= 201703L
    return std::filesystem::exists($filename);
#else
    // this is probably bugged, as the file has to "exist AND be readable",
    std::ifstream f($filename.c_str());
    return f.good();
#endif
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
#if __cplusplus >= 201703L
    return std::string(std::filesystem::current_path().string());
#else
    // about why i'm not using PATH_MAX: http://insanecoding.blogspot.com/2007/11/pathmax-simply-isnt.html
    std::string ret(0xFFFF, '\x00');
    while (::getcwd(&ret[0], ret.size()) == nullptr)
    {
        ret.resize(ret.size() * 2);
    }
    {
        const auto nullpos = ret.find('\x00');
        if (nullpos != std::string::npos)
        {
            ret.erase(nullpos);
            ret.shrink_to_fit();
        }
    }
    return ret;
#endif
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
    for (char c : $str)
    {
        out << std::setw(2) << uint_fast16_t(((unsigned char)c));
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
            ret.push_back(char(tmp));
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
double microtime(const bool $get_as_double = false)
{
    if (!$get_as_double)
    {
        throw std::logic_error("microtime(false) is not yet implemented! sorry");
    }
    return (double(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) / double(1000000));
}
std::string number_format(const double $number, const size_t $decimals = 0, const std::string &$dec_point = ".", const std::string &$thousands_sep = ",")
{
    // i don't know how to implement this function "in a good way"
    std::string ret;
    {
        std::ostringstream oss;
        oss.precision(std::streamsize($decimals));
        oss << std::fixed << $number;
        ret = oss.str();
    }
    auto dotpos = ($decimals <= 0 ? std::string::npos : ret.find("."));
    assert($decimals <= 0 || dotpos != std::string::npos);
    {
        uint_fast8_t last = 0;
        for (size_t i = ($decimals <= 0 ? ret.size() : (dotpos)); i > 0; --i)
        {
            if (last == 3)
            {
                ret.insert(i, $thousands_sep);
                dotpos+=$thousands_sep.size();
                last = 1;
            }
            else
            {
                ++last;
            }
        }
    }
    if ($decimals > 0)
    {
        ret.replace(dotpos, 1, $dec_point);
    }
    return ret;
}
std::string urlencode(const std::string &$str)
{
    std::string ossbuf;
    ossbuf.reserve($str.size() + (10 * 2)); // assume no more than 10 characters needs to be escaped (faster if true)
    std::ostringstream out(std::move(ossbuf));
    out << std::hex << std::uppercase; //  << std::setfill('\x00');// << std::setw(1);
    for (const char c : $str)
    {
        if (::isalnum(int(c)) || c == '-' || c == '_' || c == '.') // not sure why isalnum want int.. ¯\_(ツ)_/¯
        {
            out << c;
        }
        else if (c == ' ')
        {
            out << '+';
        }
        else
        {
            out << '%';
            const unsigned char uc = static_cast<unsigned char>(c);
            if (uc < '\x10')
            {
                //i bet there is a better way to do this..
                out << '0';
            }
            out << uint_fast16_t(uc);
        }
    }
    return out.str();
}
std::string rawurlencode(const std::string &$str)
{
    std::string ossbuf;
    ossbuf.reserve($str.size() + (10 * 2)); // assume no more than 10 characters needs to be escaped (faster if true)
    std::ostringstream out(std::move(ossbuf));
    out << std::hex << std::uppercase; //  << std::setfill('\x00');// << std::setw(1);
    for (const char c : $str)
    {
        if (::isalnum(int(c)) || c == '-' || c == '_' || c == '.' || c == '~') // not sure why isalnum want int.. ¯\_(ツ)_/¯
        {
            out << c;
        }
        else
        {
            out << '%';
            const unsigned char uc = static_cast<unsigned char>(c);
            if (uc < '\x10')
            {
                //i bet there is a better way to do this..
                out << '0';
            }
            out << uint_fast16_t(uc);
        }
    }
    return out.str();
}
std::string escapeshellarg(const std::string &$arg){
    std::string ret="'";
    ret.reserve($arg.length()+100+2); // ¯\_(ツ)_/¯
    for(size_t i=0;i<$arg.length();++i){
        if($arg[i]=='\00'){
            throw std::runtime_error("argument contains null bytes, impossible to escape null bytes!");
        } else if($arg[i]=='\'') {
            ret+="'\\''";
        } else {
            ret += $arg[i];
        }
    }
    ret+="'";
    return ret;
}
std::string random_bytes(std::size_t size) {
	// thread_local static is not needed, it makes the code faster, but use more ram/resources, o well
	// (for example it may contain a fopen() handle to /dev/urandom )
	thread_local static std::random_device rd;
	decltype(rd()) inner_buf;
	// optimizeme: figure out how to construct a string of uninitialized bytes,
	// the zero-initialization is just a waste of cpu
	// (think of it like this: we're using calloc() when we just need malloc())
	std::string ret(size, 0);
	char *buf = (char*) ret.data();
	while (size >= sizeof(inner_buf)) {
		size -= sizeof(inner_buf);
		inner_buf = rd();
		std::memcpy(buf, &inner_buf, sizeof(inner_buf));
		buf += sizeof(inner_buf);
	}
	if (size > 0) {
		inner_buf = rd();
		std::memcpy(buf, &inner_buf, size);
	}
	return ret;
}

int64_t random_int(const int64_t min, const int64_t max) {
	// thread_local static is not needed, it makes the code faster, but use more ram/resources, o well
	// (for example it may contain a fopen() handle to /dev/urandom )
	thread_local static std::random_device rd;
	std::uniform_int_distribution < int64_t > dist(min, max);
	return dist(rd);
}


} // namespace php
