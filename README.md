# phpcpp
php functions ported to c++

to see what's available check https://github.com/divinity76/phpcpp/blob/master/src/php_namespace.hpp#L17

# warning

the functions are imprecisely ported. While they are all at least  _roughly equivalent_, some functions do deviate from their PHP counterparts, and to make matters worse the deviations are undocumented  (for example file_get_contents() does not (yet?) support $use_include_path /  resource $context  / int $offset = 0 /  int $maxlen)

also the testsuite is not comprensive, and there is no api stability guarantee and very few functions are available and there is no version number.
