#ifndef __LOGGER_EX_H_
#define __LOGGER_EX_H_

#include <os_common.h>
#include <logger.h>
#include <str_error.h>

namespace cppbase
{

class LoggerEx
{
public:
    enum Phase : uint32_t
    {
        Init = 0,
        Start,
        Running,
        Stop,
        Exit
    };

public:
    LoggerEx() = default;
    ~LoggerEx() = default;

    void Init(const char *lpOwner, IStrError *lpStrError, ILogger *lpLogger)
    {
        m_lpStrError = lpStrError;
        m_lpLogger = lpLogger;
        m_lpOwner = lpOwner;
    };

    void SetPhase(Phase ePhase)
    {
        m_ePhase = ePhase;
        if (m_ePhase != Phase::Running)
        {
            m_eOutputFlag = Output2File | Output2Console;
        }
        else
        {
            m_eOutputFlag= Output2File;
        }
    }

    inline ILogger::LogLevel GetLogLevel() const
    {
        if (likely(m_lpLogger != nullptr))
        {
            return m_lpLogger->GetLogLevel();
        }
        return ILogger::LogLevel::Event;
    }

    const char *GetOwner()
    {
        if (likely(m_lpOwner != nullptr))
        {
            return m_lpOwner;
        }
        return "";
    }

    const char *GetPhaseStr()
    {
        static const char *PhaseStr[] = {
            "Init", "Start", "Running", "Stop", "Exit"
        };

        return PhaseStr[m_ePhase];
    }

    ILogger *GetLogger() const
    {
        return m_lpLogger;
    }

    IStrError *GetStrError() const
    {
        return m_lpStrError;
    }

    inline int32_t Log(int32_t iErrorNo, ILogger::LogLevel eLevel, const char *lppParams, uint32_t uSize)
    {
        if (likely(m_lpStrError != nullptr && m_lpLogger != nullptr && eLevel >= m_lpLogger->GetLogLevel()))
        {
            auto lpErrorMsg = m_lpStrError->StrError(iErrorNo, lppParams, uSize);
            if (likely(lpErrorMsg != nullptr))
            {
                return m_lpLogger->Log(iErrorNo, eLevel, lpErrorMsg, m_eOutputFlag);
            }
        }

        return 0;
    }

private:
    IStrError *m_lpStrError{nullptr};
    ILogger *m_lpLogger{nullptr};
    const char *m_lpOwner{nullptr};
    Phase m_ePhase{Phase::Running};
    uint32_t m_eOutputFlag{Output2File};
};

#define LOG_BASE(iErrorNo, eLevel, ...)                            \
do                                                                 \
{                                                                  \
    if (likely(eLelve >= m_lpLoggerEx->GetLogLevel()))             \
    {                                                              \
        const char *params[] = {##__VA_ARGS__,                     \
                                m_lpLoggerEx->GetOwner(),          \
                                m_lpLoggerEx->GetPhaseStr(),       \
                                __POSITION__};                     \
        m_lpLogger->Log(iErrorNo, eLevel, params, sizeof(params)); \
    }                                                              \
} while (0)

#define LOG_DEBUG(iErrorNo, ...) LOG_BASE(iErrorNo, ILogger::LogLevel::Debug, ##__VA_ARGS__)
#define LOG_INFO(iErrorNo, ...) LOG_BASE(iErrorNo, ILogger::LogLevel::Info, ##__VA_ARGS__)
#define LOG_WARN(iErrorNo, ...) LOG_BASE(iErrorNo, ILogger::LogLevel::Warn, ##__VA_ARGS__)
#define LOG_ERROR(iErrorNo, ...) LOG_BASE(iErrorNo, ILogger::LogLevel::Error, ##__VA_ARGS__)
#define LOG_FATAL(iErrorNo, ...) LOG_BASE(iErrorNo, ILogger::LogLevel::Fatal, ##__VA_ARGS__)
#define LOG_EVENT(iErrorNo, ...) LOG_BASE(iErrorNo, ILogger::LogLevel::Event, ##__VA_ARGS__)

}

#endif //__LOGGER_EX_H_