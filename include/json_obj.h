#ifndef __JSON_OBJ_H_
#define __JSON_OBJ_H_

#include <os_common.h>

namespace cppbase
{

class IJsonObj
{
public:
    enum class ObjType : uint8_t
    {
        Boolean = 0,
        Integer,
        String,
        Array,
        Object,
        Unknow
    };

    struct KvItem
    {
        const char *lpKey{nullptr};
        void *lpValue{nullptr};
    };

protected:
    virtual ~IJsonObj() = default;

public:
    virtual int32_t Init(ObjType eType) = 0;

    virtual int32_t OpenFromFile(const char *lpFile) = 0;
    
    virtual int32_t OpenFromBuffer(const char *lpBuffer) = 0;

    virtual int32_t Set(const char *lpKey, bool bValue) = 0;

    virtual int32_t Set(const char *lpKey, int64_t nValue) = 0;

    virtual int32_t Set(const char *lpKey, const char *lpValue) = 0;

    virtual IJsonObj *Set(const char *lpKey, ObjType eType) = 0;

    virtual bool Get(const char *lpKey, bool bDefaultValue = false) = 0;

    virtual int64_t Get(const char *lpKey, int64_t nDefaultValue = 0) = 0;

    virtual const char *Get(const char *lpKey, const char *lpDefaultValue = nullptr) = 0;

    virtual IJsonObj *Get(const char *lpKey) = 0;

    virtual ObjType GetType() = 0;

    virtual uint32_t GetSize() = 0;

    virtual ObjType GetType(const char *lpKey) = 0;

    virtual ObjType GetType(uint32_t uIndex) = 0;

    virtual int32_t GetKeyValue(uint32_t uIndex, KvItem *lpKvItem) = 0;

    virtual const char *GetJsonStr(bool bPretty) = 0;
};

}

#ifdef __cplusplus
extern "C"
{
#endif
    EXPORT cppbase::IJsonObj *NewJsonObject();
    EXPORT void DeleteJsonObject(cppbase::IJsonObj *lpJsonObj);
#ifdef __cplusplus
}
#endif

#endif //__JSON_OBJ_H_