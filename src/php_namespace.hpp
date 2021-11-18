#include <string>
#include <vector>
#include <limits>
#include <map>
#include <cstddef>

// <php>
namespace php
{
    std::string file_get_contents(const std::string &$filename);
    size_t file_put_contents(const std::string &$filename, const std::string &$data);
    std::string rtrim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6));
    std::string ltrim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6));
    std::string trim(std::string $str, const std::string &$character_mask = std::string("\x20\x09\x0A\x0D\x00\x0B", 6));
    std::vector<std::string> explode(const std::string &$delimiter, const std::string &$string, const size_t $limit = std::numeric_limits<size_t>::max());
    std::string implode(const std::string &$glue, const std::vector<std::string> &$pieces);
    bool file_exists(const std::string &$filename);
    std::string str_replace(const std::string &$search, const std::string &$replace, std::string $subject, size_t &$count /*=0*/);
    // overload to make $count optional
    std::string str_replace(const std::string &$search, const std::string &$replace, std::string $subject);
    std::string strtolower(std::string $string);
    std::string strtoupper(std::string $string);
    std::string getcwd(void);
    std::string bin2hex(const std::string &$str);
    std::string hex2bin(const std::string &$str);
    std::string strtr(std::string $str, const std::string &$from, const std::string &$to);
    std::string strtr(const std::string &$str, const std::map<std::string, std::string> &$replace_pairs);
    double microtime(const bool $get_as_double = false);
    std::string number_format(const double $number, const size_t $decimals = 0, const std::string &$dec_point = ".", const std::string &$thousands_sep = ",");
    std::string urlencode(const std::string &$str);
    std::string rawurlencode(const std::string &$str);
    std::string escapeshellarg(const std::string &$arg);
    std::string random_bytes(std::size_t bytes);
    int64_t random_int(const int64_t min, const int64_t max);
    bool mkdir(const std::string &$pathname, const int $mode = 0777);
} // namespace php
