echo "if you see any errors below, that's bad."
g++ -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion tests.cpp ../src/php_namespace.cpp
./a.out
echo "press enter to continue.."
read
