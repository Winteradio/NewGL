#ifndef __NEWLOG_H__
#define __NEWLOG_H__

#include <iostream>

#define LOG(__LEVEL__) std::cout << "[" << #__LEVEL__ << "]|" << __FUNCTION__ << "|" << __LINE__ << "| "

#endif