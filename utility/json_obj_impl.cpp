#include "json_obj_impl.h"
#include <error_no.h>

#define __JSON_DEBUG__
#ifndef __JSON_DEBUG__
#define RETURN(iErrorNo) \
    do                   \
    {                    \
        return iErrorNo; \
    } while (0)
#else
#define RETURN(iErrorNo)                             \
    do                                               \
    {                                                \
        PRINT_ERROR("Error Occurred: %d", iErrorNo); \
        abort();                                     \
    } while (0)
#endif

    namespace cppbase
{

CJsonObjImpl::CJsonObjImpl()
{
}

CJsonObjImpl::CJsonObjImpl(ObjType eType, const void *lpValue = nullptr)
{
    switch (eType)
    {
        case ObjType::Null:
            break;

        case ObjType::Boolean:
            m_unValue.bValue = *reinterpret_cast<const bool *>(lpValue);
            break;

        case ObjType::Integer:
            m_unValue.nValue = *reinterpret_cast<const int64_t *>(lpValue);
            break;

        case ObjType::Double:
            m_unValue.dValue = *reinterpret_cast<const double *>(lpValue);
            break;

        case ObjType::String:
            new(&m_unValue) StringValueType(reinterpret_cast<const char *>(lpValue));
            break;

        case ObjType::Array:
            new(&m_unValue) ArrayValueType;
            break;

        case ObjType::Object:
        default:
            new(&m_unValue) ObjValueType;
            break;
    }

    m_eType = eType;
}

CJsonObjImpl::~CJsonObjImpl()
{
    switch (m_eType)
    {
        case ObjType::String:
            m_unValue.strValue.~basic_string();
            break;

        case ObjType::Array:
            for (auto &item : m_unValue.arrValue)
            {
                auto lpObj = reinterpret_cast<CJsonObjImpl *>(&item);
                lpObj->~CJsonObjImpl();
            }
            m_unValue.arrValue.~vector();
            break;
        
        case ObjType::Object:
            for (auto &item : m_unValue.objValue)
            {
                auto lpObj = reinterpret_cast<CJsonObjImpl *>(&item.second);
                lpObj->~CJsonObjImpl();
            }
            m_unValue.objValue.~unordered_map();
            break;

        default:
            break;
    }
}

int32_t CJsonObjImpl::Init(ObjType eType)
{
    if (m_eType != ObjType::Unknow)
    {
        RETURN(InvaliadCall);
    }

    switch (eType)
    {
        case ObjType::Array:
            new(&m_unValue) ArrayValueType;
            break;
        
        case ObjType::Object:
            new(&m_unValue) ObjValueType;
            break;

        default:
            RETURN(InvaliadParam);
    }

    m_eType = eType;
    return 0;
}

int32_t CJsonObjImpl::AddNull(const char *lpKey)
{
    try
    {
        if (m_eType == ObjType::Object && lpKey != nullptr)
        {
            auto pair = m_unValue.objValue.emplace(lpKey, _ValueType());
            if (unlikely(!pair.second))
            {
                RETURN(InvaliadParam);
            }

            new (&pair.first->second) CJsonObjImpl(ObjType::Null);
        }
        else if (m_eType == ObjType::Array && lpKey == nullptr)
        {
            m_unValue.arrValue.emplace_back(_ValueType());
            new (&m_unValue.arrValue.back()) CJsonObjImpl(ObjType::Null);
        }
    }
    catch(...)
    {
        RETURN(MallocFailed);
    }

    return 0;
}

int32_t CJsonObjImpl::AddBool(const char *lpKey, bool bValue)
{
    try
    {
        if (m_eType == ObjType::Object && lpKey != nullptr)
        {
            auto pair = m_unValue.objValue.emplace(lpKey, _ValueType());
            if (unlikely(!pair.second))
            {
                RETURN(InvaliadParam);
            }

            new (&pair.first->second) CJsonObjImpl(ObjType::Boolean, &bValue);
        }
        else if (m_eType == ObjType::Array && lpKey == nullptr)
        {
            m_unValue.arrValue.emplace_back(_ValueType());
            new (&m_unValue.arrValue.back()) CJsonObjImpl(ObjType::Boolean, &bValue);
        }
    }
    catch(...)
    {
        RETURN(MallocFailed);
    }

    return 0;
}

int32_t CJsonObjImpl::AddInt(const char *lpKey, int64_t nValue)
{
    try
    {
        if (m_eType == ObjType::Object && lpKey != nullptr)
        {
            auto pair = m_unValue.objValue.emplace(lpKey, _ValueType());
            if (unlikely(!pair.second))
            {
                RETURN(InvaliadParam);
            }

            new (&pair.first->second) CJsonObjImpl(ObjType::Integer, &nValue);
        }
        else if (m_eType == ObjType::Array && lpKey == nullptr)
        {
            m_unValue.arrValue.emplace_back(_ValueType());
            new (&m_unValue.arrValue.back()) CJsonObjImpl(ObjType::Integer, &nValue);
        }
    }
    catch(...)
    {
        RETURN(MallocFailed);
    }

    return 0;
}

int32_t CJsonObjImpl::AddDouble(const char *lpKey, double dValue)
{
    try
    {
        if (m_eType == ObjType::Object && lpKey != nullptr)
        {
            auto pair = m_unValue.objValue.emplace(lpKey, _ValueType());
            if (unlikely(!pair.second))
            {
                RETURN(InvaliadParam);
            }

            new (&pair.first->second) CJsonObjImpl(ObjType::Double, &dValue);
        }
        else if (m_eType == ObjType::Array && lpKey == nullptr)
        {
            m_unValue.arrValue.emplace_back(_ValueType());
            new (&m_unValue.arrValue.back()) CJsonObjImpl(ObjType::Double, &dValue);
        }
    }
    catch(...)
    {
        RETURN(MallocFailed);
    }

    return 0;
}

int32_t CJsonObjImpl::AddString(const char *lpKey, const char *lpValue)
{
    try
    {
        if (m_eType == ObjType::Object && lpKey != nullptr)
        {
            auto pair = m_unValue.objValue.emplace(lpKey, _ValueType());
            if (unlikely(!pair.second))
            {
                RETURN(InvaliadParam);
            }

            new (&pair.first->second) CJsonObjImpl(ObjType::String, &lpValue);
        }
        else if (m_eType == ObjType::Array && lpKey == nullptr)
        {
            m_unValue.arrValue.emplace_back(_ValueType());
            new (&m_unValue.arrValue.back()) CJsonObjImpl(ObjType::String, &lpValue);
        }
    }
    catch(...)
    {
        RETURN(MallocFailed);
    }

    return 0;
}

IJsonObj *CJsonObjImpl::AddArray(const char *lpKey)
{
    try
    {
        if (m_eType == ObjType::Object && lpKey != nullptr)
        {
            auto pair = m_unValue.objValue.emplace(lpKey, _ValueType());
            if (unlikely(pair.second))
            {
                return new (&pair.first->second) CJsonObjImpl(ObjType::Array);
            }
        }
        else if (m_eType == ObjType::Array && lpKey == nullptr)
        {
            m_unValue.arrValue.emplace_back(_ValueType());
            return new (&m_unValue.arrValue.back()) CJsonObjImpl(ObjType::Array);
        }
    }
    catch(...)
    {
    }

    return nullptr;
}

IJsonObj *CJsonObjImpl::AddObject(const char *lpKey)
{
    try
    {
        if (m_eType == ObjType::Object && lpKey != nullptr)
        {
            auto pair = m_unValue.objValue.emplace(lpKey, _ValueType());
            if (unlikely(pair.second))
            {
                return new (&pair.first->second) CJsonObjImpl(ObjType::Object);
            }
        }
        else if (m_eType == ObjType::Array && lpKey == nullptr)
        {
            m_unValue.arrValue.emplace_back(_ValueType());
            return new (&m_unValue.arrValue.back()) CJsonObjImpl(ObjType::Object);
        }
    }
    catch(...)
    {
    }

    return nullptr;
}

bool CJsonObjImpl::GetNull(const char *lpKey)
{
    if (unlikely(lpKey == nullptr || m_eType != ObjType::Object))
    {
        return false;
    }

    auto iter = m_unValue.objValue.find(lpKey);
    if (likely(iter == m_unValue.objValue.end()))
    {
        return false;
    }

    auto lpJsonObj = reinterpret_cast<CJsonObjImpl *>(&iter->second);
    return lpJsonObj->m_eType == ObjType::Null;
}

bool CJsonObjImpl::GetBool(const char *lpKey, bool bDefaultValue)
{
    if (unlikely(lpKey == nullptr || m_eType != ObjType::Object))
    {
        return bDefaultValue;
    }

    auto iter = m_unValue.objValue.find(lpKey);
    if (likely(iter == m_unValue.objValue.end()))
    {
        return bDefaultValue;
    }

    auto lpJsonObj = reinterpret_cast<CJsonObjImpl *>(&iter->second);
    if (likely(lpJsonObj->m_eType == ObjType::Boolean))
    {
        return lpJsonObj->m_unValue.bValue;
    }
    
    return bDefaultValue;
}

int64_t CJsonObjImpl::GetInt(const char *lpKey, int64_t nDefaultValue)
{
    if (unlikely(lpKey == nullptr || m_eType != ObjType::Object))
    {
        return nDefaultValue;
    }

    auto iter = m_unValue.objValue.find(lpKey);
    if (likely(iter == m_unValue.objValue.end()))
    {
        return nDefaultValue;
    }

    auto lpJsonObj = reinterpret_cast<CJsonObjImpl *>(&iter->second);
    if (likely(lpJsonObj->m_eType == ObjType::Integer))
    {
        return lpJsonObj->m_unValue.nValue;
    }
    
    return nDefaultValue;
}

double CJsonObjImpl::GetDouble(const char *lpKey, double dDefaultValue)
{
    if (unlikely(lpKey == nullptr || m_eType != ObjType::Object))
    {
        return dDefaultValue;
    }

    auto iter = m_unValue.objValue.find(lpKey);
    if (likely(iter == m_unValue.objValue.end()))
    {
        return dDefaultValue;
    }

    auto lpJsonObj = reinterpret_cast<CJsonObjImpl *>(&iter->second);
    if (likely(lpJsonObj->m_eType == ObjType::Double))
    {
        return lpJsonObj->m_unValue.dValue;
    }
    
    return dDefaultValue;
}

const char *CJsonObjImpl::GetString(const char *lpKey, const char *lpDefaultValue)
{
    if (unlikely(lpKey == nullptr || m_eType != ObjType::Object))
    {
        return lpDefaultValue;
    }

    auto iter = m_unValue.objValue.find(lpKey);
    if (likely(iter == m_unValue.objValue.end()))
    {
        return lpDefaultValue;
    }

    auto lpJsonObj = reinterpret_cast<CJsonObjImpl *>(&iter->second);
    if (likely(lpJsonObj->m_eType == ObjType::String))
    {
        return lpJsonObj->m_unValue.strValue.c_str();
    }
    
    return lpDefaultValue;
}

IJsonObj *CJsonObjImpl::GetArray(const char *lpKey)
{
    if (unlikely(lpKey == nullptr || m_eType != ObjType::Object))
    {
        return nullptr;
    }

    auto iter = m_unValue.objValue.find(lpKey);
    if (likely(iter == m_unValue.objValue.end()))
    {
        return nullptr;
    }

    auto lpJsonObj = reinterpret_cast<CJsonObjImpl *>(&iter->second);
    if (likely(lpJsonObj->m_eType == ObjType::Array))
    {
        return reinterpret_cast<CJsonObjImpl *>(&lpJsonObj->m_unValue.objValue);
    }
    
    return nullptr;
}

IJsonObj *CJsonObjImpl::GetObject(const char *lpKey)
{
    if (unlikely(lpKey == nullptr || m_eType != ObjType::Object))
    {
        return nullptr;
    }

    auto iter = m_unValue.objValue.find(lpKey);
    if (likely(iter == m_unValue.objValue.end()))
    {
        return nullptr;
    }

    auto lpJsonObj = reinterpret_cast<CJsonObjImpl *>(&iter->second);
    if (likely(lpJsonObj->m_eType == ObjType::Object))
    {
        return reinterpret_cast<CJsonObjImpl *>(&lpJsonObj->m_unValue.objValue);
    }
    
    return nullptr;
}

bool CJsonObjImpl::IsExist(const char *lpKey)
{
    if (unlikely(lpKey == nullptr || m_eType != ObjType::Object))
    {
        return false;
    }

    return m_unValue.objValue.count(lpKey) == 1;
}

IJsonObj::ObjType CJsonObjImpl::GetType()
{
    return m_eType;
}

uint32_t CJsonObjImpl::GetSize()
{
    switch (m_eType)
    {
        case ObjType::Array:
            return m_unValue.arrValue.size();

        case ObjType::Object:
            return m_unValue.objValue.size();

        case ObjType::Boolean:
        case ObjType::Integer:
        case ObjType::String:
        default:
            break;
    }

    return 1;
}

IJsonObj::ObjType CJsonObjImpl::GetType(const char *lpKey)
{
    if (unlikely(lpKey == nullptr || m_eType != ObjType::Object))
    {
        return ObjType::Unknow;
    }

    auto iter = m_unValue.objValue.find(lpKey);
    if (likely(iter == m_unValue.objValue.end()))
    {
        return ObjType::Unknow;
    }

    auto lpJsonObj = reinterpret_cast<CJsonObjImpl *>(&iter->second);
    return lpJsonObj->m_eType;
}

IJsonObj::ObjType CJsonObjImpl::GetType(uint32_t uIndex)
{
    uint32_t begin = 0;
    switch (m_eType)
    {
        case ObjType::Array:
            if (likely(uIndex < m_unValue.arrValue.size()))
            {
                return reinterpret_cast<CJsonObjImpl *>(&m_unValue.arrValue[uIndex])->m_eType;
            }
            break;

        case ObjType::Object:
            if (uIndex < m_unValue.objValue.size())
            {
                for (auto &item : m_unValue.objValue)
                {
                    if (begin == uIndex)
                    {
                        return reinterpret_cast<CJsonObjImpl *>(&item.second)->m_eType;
                    }
                    begin++;
                }
            }
            break;

        case ObjType::Boolean:
        case ObjType::Integer:
        case ObjType::String:
        default:
            break;
    }

    return ObjType::Unknow;
}

int32_t CJsonObjImpl::GetItem(uint32_t uIndex, KvItem *lpKvItem)
{
    if (unlikely(m_eType != ObjType::Object || uIndex >= m_unValue.objValue.size()))
    {
        RETURN(InvaliadParam);
    }

    uint32_t begin = 0;
    for (auto &item : m_unValue.objValue)
    {
        if (begin == uIndex)
        {
            auto lpObj = reinterpret_cast<CJsonObjImpl *>(&item.second);
            lpKvItem->eType = lpObj->m_eType;
            lpKvItem->lpKey = item.first.c_str();
            switch (lpObj->m_eType)
            {
                case ObjType::Null:
                    lpKvItem->IsNull = true;
                    break;
                
                case ObjType::Boolean:
                    lpKvItem->bValue = lpObj->m_unValue.bValue;
                    break;

                case ObjType::Integer:
                    lpKvItem->nValue = lpObj->m_unValue.nValue;
                    break;

                case ObjType::String:
                    lpKvItem->strValue = lpObj->m_unValue.strValue.c_str();
                    break;

                case ObjType::Array:
                case ObjType::Object:
                    lpKvItem->lpArray = lpObj;

                default:
                    RETURN(InvaliadCall);
            }

            return 0;
        }
        begin++;
    }

    RETURN(InvaliadParam);
}

bool CJsonObjImpl::IsNullChar(char ch)
{
    return (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t');
}

bool CJsonObjImpl::IsNumChar(char ch)
{
    return (ch >= '0' && ch <= '9') || ch == '-' || ch == '.';
}

bool CJsonObjImpl::IsCharZero2Nine(char ch)
{
    return (ch >= '0' && ch <= '9');
}

const char  *CJsonObjImpl::ParseString(const char *lpContent, uint64_t &uIndex)
{
    uIndex++; // '"'
    char pre = '\0';
    for (; lpContent[uIndex] != '\0'; uIndex++)
    {
        if (lpContent[uIndex] == '"' && pre != '\\')
        {
            return &lpContent[uIndex++];
        }
    }

    return nullptr;
}

const char *CJsonObjImpl::ParseNumber(const char *lpContent, uint64_t &uIndex, bool &bDouble)
{
    if (unlikely(lpContent[uIndex] == '.'))
    {
        return nullptr;
    }

    if (lpContent[uIndex] == '-')
    {
        uIndex++;
    }

    bDouble = false;
    for (; lpContent[uIndex] != '\0'; uIndex++)
    {
        if (lpContent[uIndex] == '.' && IsCharZero2Nine(lpContent[uIndex + 1]))
        {
            bDouble = true;
            continue;
        }

        if (!IsCharZero2Nine(lpContent[uIndex]))
        {
            return &lpContent[uIndex++];
        }
    }
}

int32_t CJsonObjImpl::ParseKeyValue(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj)
{
    try
    {
        // "xxx": xxxx ,

        // parse key
        auto lpKeyBeing = &lpContent[uIndex + 1];
        auto lpKeyEnd = ParseString(lpContent, uIndex);
        if (lpKeyEnd == nullptr)
        {
            RETURN(ParseDataFialed);
        }
        std::string strKey(lpKeyBeing, lpKeyEnd);

        while (IsNullChar(lpContent[uIndex++]));
        if (lpContent[uIndex] != ':')
        {
            RETURN(ParseDataFialed);
        }

        // parse value
        while (IsNullChar(lpContent[uIndex++]));
        auto ch = lpContent[uIndex];
        if (ch == '{')
        {
            auto lpObj = lpJsonObj->AddObject(strKey.c_str());
            if (lpObj == nullptr)
            {
                RETURN(MallocFailed);
            }
            if (ParseObject(lpContent, uIndex, lpObj) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (ch == '[')
        {
            auto lpObj = lpJsonObj->AddArray(strKey.c_str());
            if (lpObj == nullptr)
            {
                RETURN(MallocFailed);
            }
            if (ParseArray(lpContent, uIndex, lpObj) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (ch == '"')
        {
            auto lpStrValueBegin = &lpContent[uIndex + 1];
            auto lpStrValueEnd = ParseString(lpContent, uIndex);
            std::string strValue(lpStrValueBegin, lpStrValueEnd);
            if (lpJsonObj->AddString(strKey.c_str(), strValue.c_str()) != 0)
            {
                RETURN(MallocFailed);
            }
        }
        else if (IsNumChar(ch))
        {
            bool bDouble = false;
            auto lpNumBegin = &lpContent[uIndex];
            auto lpNumEnd = ParseNumber(lpContent, uIndex, bDouble);
            if (lpNumEnd == nullptr)
            {
                RETURN(ParseDataFialed);
            }
            std::string strNum(lpNumBegin, lpNumEnd);
            if (bDouble)
            {
                auto dValue = std::atof(strNum.c_str());
                if (lpJsonObj->AddDouble(strKey.c_str(), dValue) != 0)
                {
                    RETURN(MallocFailed);
                }
            }
            else
            {
                int64_t nValue = std::atoll(strNum.c_str());
                if (lpJsonObj->AddInt(strKey.c_str(), nValue) != 0)
                {
                    RETURN(MallocFailed);
                }
            }
        }
        else if (ch == 't' || ch == 'f') // true or false
        {
            if (lpContent[uIndex] == '\0' || lpContent[uIndex + 1] != '\0'
                || lpContent[uIndex + 2] == '\0' || lpContent[uIndex + 3] == '\0')
            {
                RETURN(ParseDataFialed);
            }

            auto bValue = true;
            if (*(uint32_t *)&lpContent[uIndex] == *(const uint32_t *)"true")
            {
                if (lpJsonObj->AddBool(strKey.c_str(), true) != 0)
                {
                    RETURN(MallocFailed);
                }
            }
            else if (strncmp(&lpContent[uIndex], "false", 5) == 0)
            {
                if (lpJsonObj->AddBool(strKey.c_str(), false) != 0)
                {
                    RETURN(MallocFailed);
                }
            }
            else
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (ch == 'n') // null
        {
            if (*(uint32_t *)&lpContent[uIndex] == *(const uint32_t *)"null")
            {
                if (lpJsonObj->AddNull(strKey.c_str()) != 0)
                {
                    RETURN(MallocFailed);
                }
            }
        }
        else
        {
            RETURN(ParseDataFialed);
        }
    }
    catch(...)
    {
        RETURN(MallocFailed);
    }

    while (IsNullChar(lpContent[uIndex++]));
    if (lpContent[uIndex] == ',')
    {
        uIndex++;
    }
    while (IsNullChar(lpContent[uIndex++]));

    if (lpContent[uIndex] != '}' || lpContent[uIndex] != ']')
    {
        RETURN(ParseDataFialed);
    }

    return 0;
}

int32_t CJsonObjImpl::ParseArrNull(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj)
{
    if (*(uint32_t *)&lpContent[uIndex] == *(const uint32_t *)"null")
    {
        if (lpJsonObj->AddNull(nullptr) != 0)
        {
            RETURN(MallocFailed);
        }
    }

    while (IsNullChar(lpContent[uIndex++]));
    if (lpContent[uIndex] == ',')
    {
        uIndex++;
    }
    while (IsNullChar(lpContent[uIndex++]));

    if (lpContent[uIndex] != ']')
    {
        RETURN(ParseDataFialed);
    }

    return 0;
}

int32_t CJsonObjImpl::ParseArrBool(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj)
{
    if (lpContent[uIndex] == '\0' || lpContent[uIndex + 1] != '\0'
        || lpContent[uIndex + 2] == '\0' || lpContent[uIndex + 3] == '\0')
    {
        RETURN(ParseDataFialed);
    }

    auto bValue = true;
    if (*(uint32_t *)&lpContent[uIndex] == *(const uint32_t *)"true")
    {
        if (lpJsonObj->AddBool(nullptr, true) != 0)
        {
            RETURN(MallocFailed);
        }
    }
    else if (strncmp(&lpContent[uIndex], "false", 5) == 0)
    {
        if (lpJsonObj->AddBool(nullptr, false) != 0)
        {
            RETURN(MallocFailed);
        }
    }
    else
    {
        RETURN(ParseDataFialed);
    }

    while (IsNullChar(lpContent[uIndex++]));
    if (lpContent[uIndex] == ',')
    {
        uIndex++;
    }
    while (IsNullChar(lpContent[uIndex++]));

    if (lpContent[uIndex] != ']')
    {
        RETURN(ParseDataFialed);
    }

    return 0;
}

int32_t CJsonObjImpl::ParseArrNumber(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj)
{
    bool bDouble = false;
    auto lpNumBegin = &lpContent[uIndex];
    auto lpNumEnd = ParseNumber(lpContent, uIndex, bDouble);
    if (lpNumEnd == nullptr)
    {
        RETURN(ParseDataFialed);
    }
    std::string strNum(lpNumBegin, lpNumEnd);
    if (bDouble)
    {
        auto dValue = std::atof(strNum.c_str());
        if (lpJsonObj->AddDouble(nullptr, dValue) != 0)
        {
            RETURN(MallocFailed);
        }
    }
    else
    {
        int64_t nValue = std::atoll(strNum.c_str());
        if (lpJsonObj->AddInt(nullptr, nValue) != 0)
        {
            RETURN(MallocFailed);
        }
    }

    while (IsNullChar(lpContent[uIndex++]));
    if (lpContent[uIndex] == ',')
    {
        uIndex++;
    }
    while (IsNullChar(lpContent[uIndex++]));

    if (lpContent[uIndex] != ']')
    {
        RETURN(ParseDataFialed);
    }

    return 0;
}

int32_t CJsonObjImpl::ParseArrString(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj)
{
    try
    {
        auto lpStrValueBegin = &lpContent[uIndex + 1];
        auto lpStrValueEnd = ParseString(lpContent, uIndex);
        std::string strValue(lpStrValueBegin, lpStrValueEnd);
        if (lpJsonObj->AddString(nullptr, strValue.c_str()) != 0)
        {
            RETURN(MallocFailed);
        }
    }
    catch(...)
    {
        RETURN(MallocFailed);
    }

    while (IsNullChar(lpContent[uIndex++]));
    if (lpContent[uIndex] == ',')
    {
        uIndex++;
    }
    while (IsNullChar(lpContent[uIndex++]));

    if (lpContent[uIndex] != ']')
    {
        RETURN(ParseDataFialed);
    }

    return 0;
}

int32_t CJsonObjImpl::ParseArray(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj)
{
    // "[......]"
    uIndex++; // '['
    for (;lpContent[uIndex] != '\0'; uIndex++)
    {
        auto ch = lpContent[uIndex];
        if (IsNullChar(ch))
        {
            continue;
        }
        else if (ch == 'n') // null
        {
            if (ParseArrNull(lpContent, uIndex, lpJsonObj) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (ch == 't' || ch == 'f') // true or false
        {
            if (ParseArrBool(lpContent, uIndex, lpJsonObj) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (ch == '"')
        {
            if (ParseArrString(lpContent, uIndex, lpJsonObj) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (IsNumChar(ch))
        {
            if (ParseArrNumber(lpContent, uIndex, lpJsonObj) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (ch == '[')
        {
            auto lpArray = lpJsonObj->AddArray(nullptr);
            if (ParseArray(lpContent, uIndex, lpArray) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (ch == '{')
        {
            auto lpObj = lpJsonObj->AddObject(nullptr);
            if (ParseObject(lpContent, uIndex, lpObj) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (ch == ']')
        {
            uIndex++;
            break;
        }
        else
        {
            RETURN(ParseDataFialed);
        }
    }

    return 0;
}

int32_t CJsonObjImpl::ParseObject(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj)
{
    // "{......}"
    uIndex++; // '{'
    for (;lpContent[uIndex] != '\0'; uIndex++)
    {
        auto ch = lpContent[uIndex];
        if (IsNullChar(ch))
        {
            continue;
        }
        else if (ch == '"')
        {
            if (ParseKeyValue(lpContent, uIndex, lpJsonObj) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (ch == '}')
        {
            uIndex++;
            break;
        }
        else
        {
            RETURN(ParseDataFialed);
        }
    }

    return 0;
}

int32_t CJsonObjImpl::ParseContent(const char *lpContent)
{
    uint64_t uIndex = 0;
    int32_t iErrorNo = 0;
    while (lpContent[uIndex] != '\0')
    {
        if (IsNullChar(lpContent[uIndex]))
        {
            continue;
        }
        else if (lpContent[uIndex] == '{' )
        {
            if (ParseObject(lpContent, uIndex, this) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else if (lpContent[uIndex] == '[')
        {
            if (ParseArray(lpContent, uIndex, this) != 0)
            {
                RETURN(ParseDataFialed);
            }
        }
        else
        {
            RETURN(ParseDataFialed);
        }
    }

    return 0;
}

int32_t CJsonObjImpl::OpenFromFile(const char *lpFile)
{
    if (unlikely(lpFile == nullptr))
    {
        RETURN(InvaliadParam);
    }

    auto lpHandler = fopen(lpFile, "r");
    if (lpHandler == nullptr)
    {
        RETURN(OpenFileFailed);
    }

    int32_t iErrorNo = 0;
    char szLine[4096];
    while (fgets(szLine, sizeof(szLine), lpHandler) != nullptr)
    {
        iErrorNo = ParseContent(szLine);
        if (iErrorNo != 0)
        {
            break;
        }
    }

    fclose(lpHandler);
    RETURN(iErrorNo);
}

int32_t CJsonObjImpl::OpenFromBuffer(const char *lpBuffer)
{
    if (unlikely(lpBuffer == nullptr))
    {
        RETURN(InvaliadParam);
    }

    return ParseContent(lpBuffer);
}

const char *CJsonObjImpl::GetJsonStr(bool bPretty)
{

}

}

cppbase::IJsonObj *NewJsonObject()
{
    return NEW cppbase::CJsonObjImpl();
}

void DeleteJsonObject(cppbase::IJsonObj *lpJsonObj)
{
    delete (cppbase::CJsonObjImpl *)lpJsonObj;
}