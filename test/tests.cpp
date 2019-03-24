// TODO: make a windows-friendly version
#include <iostream>
//#include <error.h> // gcc-specific..
//#include <execinfo.h>//linux-specific..
#include "../src/php_namespace.cpp"
using namespace std;

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
#define ra(exp)                                                                 \
	{                                                                           \
		if (!(exp))                                                             \
		{                                                                       \
			std::cerr << __FILE__ << ":" << __LINE__ << " error!" << std::endl; \
			exit(EXIT_FAILURE);                                                 \
		}                                                                       \
	}

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
static void run()
{
	cout << "explode() tests: " << flush;
	explode_tests();
	cout << "OK" << endl;
}

int main()
{
	run();
	return 0;
}