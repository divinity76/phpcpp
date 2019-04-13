// TODO: make a windows-friendly version
#include <iostream>
#include "../src/php_namespace.hpp"
//#include <error.h> // gcc-specific..
//#include <execinfo.h>//linux-specific..
using namespace std;

#define BIN_STRING(s) std::string(s, sizeof(s) - 1)

/*
#define macrobacktrace() { \
void *array[20]; \
 int traces=backtrace(array,sizeof(array)/sizeof(array[0])); \
 if(traces<=0) { \
 	fprintf(stderr,"failed to get a backtrace!"); \
 } else { \
 backtrace_symbols_fd(array,traces,STDERR_FILENO); \
 } \
}

// #define myerror(status,errnum,...){macrobacktrace();error_at_line(status,errnum,__FILE__,__LINE__,__VA_ARGS__);}

// #define ra2(exp,...){if(!(exp)){myerror(1,errno,__VA_ARGS__);}}
*/
// "runtime assert"

void dump_string(const std::string &str)
{
	// string(3) "lol"
	std::cerr << "string(" << str.size() << "): \"" << str << "\"" << std::endl;
}
#define ra(exp)                                                                                      \
	{                                                                                                \
		if (!(exp))                                                                                  \
		{                                                                                            \
			const std::string s(std::string(__FILE__) + ":" + std::to_string(__LINE__) + " error!"); \
			std::cerr << s << std::endl;                                                             \
			throw std::runtime_error(s);                                                             \
		}                                                                                            \
	}

const static std::string everything("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff", 256);
const static std::string everything_hex_lowercase("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff");
const static std::string everything_hex_uppercase("000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF");

static void explode_tests()
{
	{
		const auto lines = php::explode("lol", "trololo");
		ra(lines.size() == 2);
		ra(lines[0] == "tro");
		ra(lines[1] == "o");
	}
	{
		const string test = "the quick brown fox";
		const auto lines = php::explode("not on my string", test);
		ra(lines.size() == 1);
		ra(lines[0] == test);
	}
}
static void bin2hex_tests()
{
	ra(php::bin2hex(everything) == everything_hex_lowercase);
	ra(php::bin2hex("THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG") == "54484520515549434b2042524f574e20464f58204a554d5053204f56455220544845204c415a5920444f47");
	ra(php::bin2hex(BIN_STRING("\x00")) == "00");
	ra(php::bin2hex(BIN_STRING("\xFF")) == "ff");
}
static void hex2bin_tests()
{
	ra(php::hex2bin(everything_hex_lowercase) == everything);
}
static void strtr_tests1()
{
	const std::string in = "aabbccXYZ";
	std::string out = php::strtr(in, "abc", "123");
	ra(out == "112233XYZ");
}
static void strtr_tests2()
{
	{
		const std::string in = "aabbccXYZ";
		std::string out = php::strtr(in, {{"a", "1"}, {"b", "2"}, {"c", "3"}});
		ra(out == "112233XYZ");
	}
	{
		const std::string in = "aabbccXYZ";
		std::string out = php::strtr(in, {{"a", "TROLO"}, {"b", ""}, {"c", ""}});
		ra(out == "TROLOTROLOXYZ");
	}
	{
		const std::string in = "aabbcc";
		std::string out = php::strtr(in, {{"a", ""}, {"b", ""}, {"c", ""}, {"not", "inhere"}});
		ra(out.empty());
	}
}
static void trim_tests(void)
{
	ra(everything.front() == '\x00');
	ra(everything.back() == '\xFF');
	//<ltrim>
	ra(php::ltrim(everything) != everything);
	ra(php::ltrim(everything, "X") == everything);
	ra(php::ltrim(everything, "") == everything);
	ra(php::ltrim(everything, BIN_STRING("\x00")) == everything.substr(1));
	//</ltrim>
	//<rtrim>
	ra(php::rtrim(everything) == everything);
	ra(php::rtrim(everything, "X") == everything);
	ra(php::rtrim(everything, "") == everything);
	ra(php::rtrim(everything, BIN_STRING("\xFF")) == everything.substr(0, everything.length() - 1));
	//</rtrim>
	//<trim>
	ra(php::trim(everything) != everything);
	ra(php::trim(everything) == everything.substr(1));
	ra(php::trim(everything, "X") == everything);
	ra(php::trim(everything, "") == everything);
	ra(php::trim(everything, BIN_STRING("\x00")) == everything.substr(1));
	ra(php::trim(everything, BIN_STRING("\x00")) == everything.substr(1));
	//</trim>
}

static void microtime_true_tests(void)
{
	const double t = php::microtime(true);
	if (t < 1262386800)
	{
		std::cerr << "WARNING: microtime(true) believes that it's earlier than year 2010! ..." << std::endl;
	}
	if (t > 4070991600)
	{
		std::cerr << "WARNING: microtime(true) believes that it's after year 2099! ..." << std::endl;
	}
}
static void number_format_tests(void)
{
	//	cout << " supposed to be ???: " << number_format(1337/double(6),99) << (number_format(1337/double(6),99) == "222.833" ? " OK" : " FAIL" ) << std::endl;
	ra(php::number_format(137, 0) == "137");
	ra(php::number_format(1337 / double(6), 0) == "223");
	ra(php::number_format(1337 / double(6), 3) == "222.833");
	ra(php::number_format(0, 9) == "0.000000000");
	ra(php::number_format(0) == "0");
	ra(php::number_format(1, 0) == "1");
	ra(php::number_format(13, 0) == "13");
	ra(php::number_format(137, 0) == "137");
	ra(php::number_format(1337, 0) == "1,337");
	ra(php::number_format(13337, 0) == "13,337");
	ra(php::number_format(133337, 0) == "133,337");
	ra(php::number_format(13333337, 0) == "13,333,337");
}
static void urlencode_tests(void)
{
	{
		const std::string q = "hi";		//BIN_STRING("\xFF\x00\xFFlol");
		const std::string q_answer = q; //"%ff%00%fflol";
		ra(php::urlencode(q) == q_answer);
	}
	{
		const std::string q = BIN_STRING("\x00\xFF\x00hi");		//BIN_STRING("\xFF\x00\xFFlol");
		const std::string q_answer = BIN_STRING("%00%FF%00hi"); //"%ff%00%fflol";
		/*dump_string(q);
		dump_string(php::urlencode(q_answer));
		dump_string(q_answer);
		*/
		ra(php::urlencode(q) == q_answer);
	}
	{
		const std::string q = everything;
		// q_answer was made with PHP.
		const std::string q_answer = "%00%01%02%03%04%05%06%07%08%09%0A%0B%0C%0D%0E%0F%10%11%12%13%14%15%16%17%18%19%1A%1B%1C%1D%1E%1F+%21%22%23%24%25%26%27%28%29%2A%2B%2C-.%2F0123456789%3A%3B%3C%3D%3E%3F%40ABCDEFGHIJKLMNOPQRSTUVWXYZ%5B%5C%5D%5E_%60abcdefghijklmnopqrstuvwxyz%7B%7C%7D%7E%7F%80%81%82%83%84%85%86%87%88%89%8A%8B%8C%8D%8E%8F%90%91%92%93%94%95%96%97%98%99%9A%9B%9C%9D%9E%9F%A0%A1%A2%A3%A4%A5%A6%A7%A8%A9%AA%AB%AC%AD%AE%AF%B0%B1%B2%B3%B4%B5%B6%B7%B8%B9%BA%BB%BC%BD%BE%BF%C0%C1%C2%C3%C4%C5%C6%C7%C8%C9%CA%CB%CC%CD%CE%CF%D0%D1%D2%D3%D4%D5%D6%D7%D8%D9%DA%DB%DC%DD%DE%DF%E0%E1%E2%E3%E4%E5%E6%E7%E8%E9%EA%EB%EC%ED%EE%EF%F0%F1%F2%F3%F4%F5%F6%F7%F8%F9%FA%FB%FC%FD%FE%FF";
		ra(php::urlencode(q) == q_answer);
	}
}
static void run()
{
	cout << "explode() tests: " << flush;
	explode_tests();
	cout << "OK" << endl;
	cout << "bin2hex() tests: " << flush;
	bin2hex_tests();
	cout << "OK" << endl;
	cout << "hex2bin() tests: " << flush;
	hex2bin_tests();
	cout << "OK" << endl;
	cout << "strtr() tests1: " << flush;
	strtr_tests1();
	cout << "OK" << endl;
	cout << "strtr() tests2: " << flush;
	strtr_tests2();
	cout << "OK" << endl;
	cout << "trim() ltrim() rtrim() tests: " << std::flush;
	trim_tests();
	cout << "OK" << endl;
	cout << "microtime(true) tests: " << flush;
	microtime_true_tests();
	cout << "OK" << endl;
	cout << "number_format() tests: " << flush;
	number_format_tests();
	cout << "OK" << endl;
	cout << "urlencode() tests: " << flush;
	urlencode_tests();
	cout << "OK" << endl;
}

int main()
{
	run();
	return 0;
}