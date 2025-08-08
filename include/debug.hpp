#pragma once
#include <iostream>

#ifdef DEBUG
#  define LOG(msg) std::cout << msg << std::endl
#else
#  define LOG(msg)
#endif
