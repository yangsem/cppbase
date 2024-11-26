#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <os_common.h>

namespace cppbase
{

class ILogger
{
public:
    enum class LogLevel : uint8_t
    {
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        Event
    };

    #define Output2File 0x01
    #define Output2Console 0x02
    #define Output2System 0x02

protected:
    virtual ~ILogger() = default;

public:
    inline LogLevel GetLogLevel() const { return m_eLevel; }

    virtual void SetLogLevel(LogLevel eLevel) = 0;

    virtual int32_t Log(int32_t iErrorNo, LogLevel eLevel, const char *lpErrorMsg, uint32_t uOutputFlag) = 0;

    virtual const char *GetStatis() = 0;

protected:
    LogLevel m_eLevel{LogLevel::Info};
};

}

#endif //__LOGGER_H_
