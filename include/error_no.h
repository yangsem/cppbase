#ifndef __ERROR_NO_H_
#define __ERROR_NO_H_

#include <stdint.h>

namespace cppbase
{

constexpr int32_t MallocFailed = 100;
constexpr int32_t InvaliadParam = 101;
constexpr int32_t SysCallFailed = 102;
constexpr int32_t InvaliadCall = 103;
constexpr int32_t OpenFileFailed = 104;
constexpr int32_t OpenDirFailed = 105;
constexpr int32_t CreateFileFailed = 106;
constexpr int32_t CreateDirFailed = 107;
constexpr int32_t ParseDataFialed = 108;

}

#endif //__ERROR_NO_H_
