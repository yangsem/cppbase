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
        case ObjType::Boolean:
            m_unValue.bValue = *(bool *)lpValue;
            break;

        case ObjType::Integer:
            m_unValue.nValue = *(int64_t *)lpValue;
            break;

        case ObjType::String:
            new(&m_unValue) StringValueType((const char *)lpValue);
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

int32_t CJsonObjImpl::Set(const char *lpKey, bool bValue)
{
    if (unlikely(lpKey == nullptr))
    {
        return InvaliadParam;
    }

    if (unlikely(m_eType != ObjType::Object))
    {
        return InvaliadCall;
    }

    try
    {
        auto pair = m_unValue.objValue.emplace(lpKey, CJsonObjImpl(ObjType::Boolean, &bValue));
        if (unlikely(!pair.second))
        {
            return InvaliadParam;
        }
    }
    catch(...)
    {
        return MallocFailed;
    }

    return 0;
}

int32_t CJsonObjImpl::Set(const char *lpKey, int64_t nValue)
{
    if (unlikely(lpKey == nullptr))
    {
        return InvaliadParam;
    }

    if (unlikely(m_eType != ObjType::Object))
    {
        return InvaliadCall;
    }

    try
    {
        auto pair = m_unValue.objValue.emplace(lpKey, CJsonObjImpl(ObjType::Integer, &nValue));
        if (unlikely(!pair.second))
        {
            return InvaliadParam;
        }
    }
    catch(...)
    {
        return MallocFailed;
    }

    return 0;
}

int32_t CJsonObjImpl::Set(const char *lpKey, const char *lpValue)
{
    if (unlikely(lpKey == nullptr || lpValue == nullptr))
    {
        return InvaliadParam;
    }

    if (unlikely(m_eType != ObjType::Object))
    {
        return InvaliadCall;
    }

    try
    {
        auto pair = m_unValue.objValue.emplace(lpKey, CJsonObjImpl(ObjType::String, &lpValue));
        if (unlikely(!pair.second))
        {
            return InvaliadParam;
        }
    }
    catch(...)
    {
        return MallocFailed;
    }

    return 0;
}

IJsonObj *CJsonObjImpl::Set(const char *lpKey, ObjType eType)
{
    if (unlikely(lpKey == nullptr))
    {
        return nullptr;
    }

    try
    {
        if (m_eType == ObjType::Array)
        {
            m_unValue.arrValue.push_back(_ValueType());
            return new(&m_unValue.arrValue.back()) CJsonObjImpl(ObjType::Array);
        }
        else if (m_eType == ObjType::Object)
        {
            auto pair = m_unValue.objValue.emplace(lpKey, _ValueType());
            if (unlikely(pair.second))
            {
                return new(&pair.first->second) CJsonObjImpl(ObjType::Object);
            }
        }
    }
    catch(...)
    {
    }

    return nullptr;
}

bool CJsonObjImpl::Get(const char *lpKey, bool bDefaultValue)
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

int64_t CJsonObjImpl::Get(const char *lpKey, int64_t nDefaultValue)
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
    if (likely(lpJsonObj->m_eType == ObjType::Boolean))
    {
        return lpJsonObj->m_unValue.nValue;
    }
    
    return nDefaultValue;
}

const char *CJsonObjImpl::Get(const char *lpKey, const char *lpDefaultValue)
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
    if (likely(lpJsonObj->m_eType == ObjType::Boolean))
    {
        return lpJsonObj->m_unValue.strValue.c_str();
    }
    
    return lpDefaultValue;
}

IJsonObj *CJsonObjImpl::Get(const char *lpKey)
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
    if (likely(lpJsonObj->m_eType == ObjType::Boolean))
    {
        return reinterpret_cast<CJsonObjImpl *>(&lpJsonObj->m_unValue.objValue);
    }
    
    return nullptr;
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

int32_t CJsonObjImpl::GetKeyValue(uint32_t uIndex, KvItem *lpKvItem)
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
            lpKvItem->lpKey = item.first.c_str();
            lpKvItem->lpValue = &item.second;
            return 0;
        }
        begin++;
    }
}

int32_t CJsonObjImpl::OpenFromFile(const char *lpFile)
{

}

int32_t CJsonObjImpl::OpenFromBuffer(const char *lpBuffer)
{

}

const char *CJsonObjImpl::GetJsonStr(bool bPretty)
{

}

}