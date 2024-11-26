#ifndef __STR_ERROR_H_
#define __STR_ERROR_H_

#include <stdint.h>
#include <stdio.h>

class ToStr
{
public:
    ToStr(char ch)
    {
        m_szBuffer[0] = ch;
        m_szBuffer[1] = '\0';
    }

    ToStr(int8_t num)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%d", num);
    }

    ToStr(uint8_t num)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%u", num);
    }

    ToStr(int16_t num)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%d", num);
    }

    ToStr(uint16_t num)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%u", num);
    }

    ToStr(int32_t num)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%d", num);
    }

    ToStr(uint32_t num)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%u", num);
    }

    ToStr(int64_t num)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%ld", num);
    }

    ToStr(uint64_t num)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%lu", num);
    }

    ToStr(double num)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%.lf", num);
    }

    ToStr(const char *lpStr)
    {
        snprintf(m_szBuffer, sizeof(m_szBuffer), "%s", lpStr);
    }

    operator const char *() const
    {
        return m_szBuffer;
    }

public:
    char m_szBuffer[32];
};

class IStrError
{
protected:
    virtual ~IStrError() = default;

public:
    virtual int32_t Load(const char *lpFile) = 0;

    virtual int32_t Add(int32_t iErrorNo, const char *lpErrorStr) = 0;

    virtual const char *StrError(int32_t iErrorNo, const char *lppParams, uint32_t uSize) = 0;
};

#endif //__STR_ERROR_H_