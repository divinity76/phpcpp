#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>

namespace php
{
#ifdef __linux__
    // execute a sysem() command and return the output as string
#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
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
        const int ret_size = ftell(tmpfile_file);
        rewind(tmpfile_file);
        std::string ret(ret_size, '\00');
        size_t remaining = ret_size;
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
#endif // __linux__
}

int main(int argc, char **argv)
{
    int return_code;
    std::string fromcmd = php::shell_exec(std::string("cat " + std::string(argv[0])));;
    std::cout << "PAPAAAAA" << fromcmd << std::flush;
    exit(0);
}