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
#include <cstring>

#if __cplusplus < 201703L
#include <sys/stat.h>
#include <sys/types.h>
#endif

// <php>
namespace php
{
#include <string>
#include <fstream>
#include <sstream>
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
        std::string result(size_t(ssize), 0);
        file.read(&result[0], std::streamsize(ssize));
        return result;
    }

#include <string>
#include <fstream>
    size_t file_put_contents(const std::string &$filename, const std::string &$data)
    {
        // TODO: implement flags/flock/append
        // right now just pretend $flags = 0
        std::ofstream file($filename, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
        file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        file.write($data.data(), std::streamsize($data.size()));
        // well.. should throw exception if everything wasn't written, so this is correct, right?
        return $data.size();
    }

#include <string>
    std::string rtrim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6))
    {
        // optimizeme: can this be optimized to a single erase() call? probably.
        while ($str.size() > 0 && $character_mask.find_first_of($str.back()) != std::string::npos)
        {
            $str.pop_back();
        }
        return $str;
    }

#include <string>
    std::string ltrim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6))
    {
        // optimizeme: can this be optimized to a single erase() call? probably.
        while ($str.size() > 0 && $character_mask.find_first_of($str.front()) != std::string::npos)
        {
            $str.erase(0, 1);
        }
        return $str;
    }

#include <string>
    std::string trim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6))
    {
        return rtrim(ltrim($str, $character_mask), $character_mask);
    }

#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
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

#include <string>
#include <vector>
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

#if __cplusplus >= 201703L
#include <filesystem>
#else
#include <fstream>
#endif
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

#include <string>
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

#if __cplusplus >= 201703L
#include <filesystem>
#else
#include <unistd.h>
#endif
#include <string>
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
#include <string>
#include <algorithm>
#include <cctype>
    std::string strtolower(std::string $string)
    {
        std::transform($string.begin(), $string.end(), $string.begin(), ::tolower);
        return $string;
    }
#include <string>
#include <algorithm>
#include <cctype>
    std::string strtoupper(std::string $string)
    {
        std::transform($string.begin(), $string.end(), $string.begin(), ::toupper);
        return $string;
    }

std::string bin2hex(const std::string_view bin)
{
    std::string result(bin.size() * 2, '\x00');
    for (size_t i = 0; i < bin.size(); ++i)
    {
        const uint8_t byte = bin[i];
        result[2 * i] = "0123456789ABCDEF"[byte >> 4];
        result[(2 * i) + 1] = "0123456789ABCDEF"[byte & 0x0F];
    }
    return result;
}
#include<stdexcept>
std::string hex2bin(const std::string_view hex)
{
    auto hexValue = [](uint8_t chr) -> uint8_t
    {
        if (chr >= '0' && chr <= '9')
            return chr - '0';
        if (chr >= 'A' && chr <= 'F')
            return chr - 'A' + 10;
        if (chr >= 'a' && chr <= 'f')
            return chr - 'a' + 10;
        throw std::invalid_argument("Invalid hex character: " + chr);
    };
    if (hex.size() % 2 != 0)
        throw std::invalid_argument("Invalid hex string: odd length");

    std::string result(hex.size() / 2, '\x00');
    for (size_t i = 0; i < result.size(); ++i)
    {
        uint8_t high = hexValue(hex[2 * i]);
        uint8_t low = hexValue(hex[2 * i + 1]);
        result[i] = static_cast<uint8_t>((high << 4) | low);
    }
    return result;
}

#include <stdexcept>
#include <string>
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

#include <vector>
#include <map>
#include <algorithm>
#include <string>
    std::string strtr(const std::string &$str, const std::map<std::string, std::string> &$replace_pairs)
    {
        //this function is not optimized, a much faster implementation is probably possible to make.
        std::vector<std::string> keys_sorted;
        keys_sorted.reserve($replace_pairs.size());
        for (auto it = $replace_pairs.begin(); it != $replace_pairs.end(); ++it)
        {
            keys_sorted.push_back(it->first);
        }
        std::sort(keys_sorted.begin(), keys_sorted.end(), [](const std::string &s1, const std::string &s2) -> bool
                  { return s1.size() > s2.size(); });
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

#include <stdexcept>
#include <chrono>
    double microtime(const bool $get_as_double = false)
    {
        if (!$get_as_double)
        {
            throw std::logic_error("microtime(false) is not yet implemented! sorry");
        }
        return (double(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) / double(1000000));
    }

#include <string>
#include <sstream>
#include <cassert>
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
                    dotpos += $thousands_sep.size();
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

#include <string>
#include <sstream>
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

#include <string>
#include <sstream>
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

#include <string>
#include <stdexcept>
std::string escapeshellarg(const std::string &$arg){
    std::string ret="'";
    ret.reserve($arg.length()+20); // ¯\_(ツ)_/¯
    for(size_t i=0;i<$arg.length();++i){
        if($arg[i]=='\00'){
            throw std::runtime_error("argument contains null bytes, impossible to escape null bytes!");
        } else if($arg[i]=='\'') {
            ret+="'\\''";
        } else {
            ret += $arg[i];
        }
    }
    ret += "'";
    return ret;
}

#include <string>
#include <cstring>
#include <random>
    std::string random_bytes(std::size_t size)
    {
        // thread_local static is not needed, it makes the code faster, but use more ram/resources, o well
        // (for example it may contain a fopen() handle to /dev/urandom )
        thread_local static std::random_device rd;
        decltype(rd()) inner_buf;
        // optimizeme: figure out how to construct a string of uninitialized bytes,
        // the zero-initialization is just a waste of cpu
        // (think of it like this: we're using calloc() when we just need malloc())
        std::string ret(size, 0);
        char *buf = (char *)ret.data();
        while (size >= sizeof(inner_buf))
        {
            size -= sizeof(inner_buf);
            inner_buf = rd();
            std::memcpy(buf, &inner_buf, sizeof(inner_buf));
            buf += sizeof(inner_buf);
        }
        if (size > 0)
        {
            inner_buf = rd();
            std::memcpy(buf, &inner_buf, size);
        }
        return ret;
    }

#include <random>
    int64_t random_int(const int64_t min, const int64_t max)
    {
        // thread_local static is not needed, it makes the code faster, but use more ram/resources, o well
        // (for example it may contain a fopen() handle to /dev/urandom )
        thread_local static std::random_device rd;
        std::uniform_int_distribution<int64_t> dist(min, max);
        return dist(rd);
    }

#include <string>
#if __cplusplus >= 201703L
#include <filesystem>
#else
#include <cstring>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#endif
    bool mkdir(const std::string &$pathname, const int $mode = 0777)
    {
// todo: support int $mode, bool $recursive
#if __cplusplus >= 201703L
        std::filesystem::create_directory($pathname);
        return true;
#else
        const int success = ::mkdir($pathname.c_str(), mode_t($mode));
        if (success == 0)
        {
            return true;
        }
        const std::string error_string = "mkdir failed, pathname: " + $pathname + " errno: " + std::to_string(errno) + " strerror: " + std::string(strerror(errno));
        throw std::runtime_error(error_string);
#endif
    }

#ifdef __linux__
    // execute a sysem() command and return the output as string
    // ... todo, add support for other systems
    // todo: rewrite linux version to use forkpty() instead..
//#include <stdio.h>
//#include <stdexcept>
// #include <iostream>
//#include <unistd.h>
    std::string shell_exec(const std::string &cmd, int &return_code)
    {
        // basic idea: duplicate stdout, close stdout, tmpfile() a new stdout with id 1, do your system() thing, then restore stdout...
        // todo: rewrite to use
        constexpr int STDOUT_MAGIC_NUMBER = 1;
        const int stdout_copy = dup(STDOUT_MAGIC_NUMBER);
        if (stdout_copy == -1)
        {
            throw std::runtime_error("dup failed to copy stdout!");
        }
        if (-1 == close(STDOUT_MAGIC_NUMBER))
        {
            close(stdout_copy);
            throw std::runtime_error("close failed to close original stdout!");
        }
        FILE *tmpfile_file = tmpfile();
        if (tmpfile_file == nullptr)
        {
            dup(stdout_copy); //pray it worked and got id 1
            close(stdout_copy);
            throw std::runtime_error("tmpfile failed to create a new stdout!");
        }
        const int tmpfile_fileno = fileno(tmpfile_file);
        if (tmpfile_fileno != STDOUT_MAGIC_NUMBER)
        {
            // to be clear, this *shouldn't happen*
            dup(stdout_copy);   // restore stdout... and pray it got id 1.. i find that unlikely
            close(stdout_copy); //pray it worked..
            fclose(tmpfile_file);
            throw std::runtime_error("tmpfile did not get id 1! it got id " + std::to_string(tmpfile_fileno));
        }
        return_code = system(cmd.c_str());
        const size_t ret_size = size_t(ftell(tmpfile_file));
        rewind(tmpfile_file);
        std::string ret((ret_size), '\00');
        size_t remaining = (ret_size);
        while (remaining > 0)
        {
            const size_t read_size = fread(&ret[ret_size - remaining], 1, remaining, tmpfile_file);
            remaining -= read_size;
            if (read_size == 0)
            {
                throw std::runtime_error("remaining was non-zero but cannot read more! wtf! im so tired of writing error checking code");
            }
        }
        fclose(tmpfile_file);
        dup(stdout_copy); // restore stdout... and pray it got id 1..
        close(stdout_copy);
        return ret;
    }
    std::string shell_exec(const std::string &cmd)
    {
        int dummy_return_code;
        return shell_exec(cmd, dummy_return_code);
    }
#endif // __linux__ for shell_exec()

} // namespace php
