#include "json_obj_impl.h"
#include <error_no.h>

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
    switch (eType)
    {
        case ObjType::Array:
            new(&m_unValue) ArrayValueType;
            break;
        
        case ObjType::Object:
            new(&m_unValue) ObjValueType;
            break;

        default:
            return InvaliadParam;
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
                return InvaliadParam;
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
        return MallocFailed;
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
                return InvaliadParam;
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
        return MallocFailed;
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
                return InvaliadParam;
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
        return MallocFailed;
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
                return InvaliadParam;
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
        return MallocFailed;
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
                return InvaliadParam;
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
        return MallocFailed;
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
        return InvaliadParam;
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
                    return InvaliadCall;
            }

            return 0;
        }
        begin++;
    }

    return InvaliadParam;
}

bool CJsonObjImpl::IsNullChar(char ch)
{
    return (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t');
}

bool CJsonObjImpl::IsNumChar(char ch)
{
    return (ch >= '0' && ch <= '9') || ch == '-' || ch == '.';
}

int32_t CJsonObjImpl::ParseArray(const char *lpContent, uint64_t &uIndex, CJsonObjImpl *lpJsonObj)
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
        else if (ch == '"')
        {
            if (ParseString(lpContent, uIndex, lpJsonObj) != 0)
            {
                return ParseDataFialed;
            }
        }
        else if (IsNumChar(ch))
        {
            if (ParseNumber(lpContent, uIndex, lpJsonObj) != 0)
            {
                return ParseDataFialed;
            }
        }
        else if (ch == '[')
        {
            auto lpArray = (CJsonObjImpl *)lpJsonObj->AddArray(nullptr);
            if (ParseArray(lpContent, uIndex, lpArray) != 0)
            {
                return ParseDataFialed;
            }
        }
        else if (ch == '{')
        {
            auto lpObj = (CJsonObjImpl *)lpJsonObj->AddObject(nullptr);
            if (ParseObject(lpContent, uIndex, lpObj) != 0)
            {
                return ParseDataFialed;
            }
        }
        else if (ch == ']')
        {
            uIndex++;
            break;
        }
        else
        {
            return ParseDataFialed;
        }
    }

    return 0;
}

int32_t CJsonObjImpl::ParseObject(const char *lpContent, uint64_t &uIndex, CJsonObjImpl *lpJsonObj)
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
                return ParseDataFialed;
            }
        }
        else if (ch == '}')
        {
            uIndex++;
            break;
        }
        else
        {
            return ParseDataFialed;
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
                return ParseDataFialed;
            }
        }
        else if (lpContent[uIndex] == '[')
        {
            if (ParseArray(lpContent, uIndex, this) != 0)
            {
                return ParseDataFialed;
            }
        }
        else
        {
            return ParseDataFialed;
        }
    }

    return 0;
}

int32_t CJsonObjImpl::OpenFromFile(const char *lpFile)
{
    if (unlikely(lpFile == nullptr))
    {
        return InvaliadParam;
    }

    auto lpHandler = fopen(lpFile, "r");
    if (lpHandler == nullptr)
    {
        return OpenFileFailed;
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
    return iErrorNo;
}

int32_t CJsonObjImpl::OpenFromBuffer(const char *lpBuffer)
{
    if (unlikely(lpBuffer == nullptr))
    {
        return InvaliadParam;
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