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
        Null = 0,
        Boolean,
        Integer,
        Double,
        String,
        Array,
        Object,
        Unknow
    };

    struct KvItem
    {
        ObjType eType;
        const char *lpKey{nullptr};
        union {
            bool IsNull;
            bool bValue;
            int64_t nValue;
            const char *strValue;
            IJsonObj *lpArray;
            IJsonObj *lpObj;
        };
    };

protected:
    virtual ~IJsonObj() = default;

public:
    virtual int32_t Init(ObjType eType) = 0;

    virtual int32_t OpenFromFile(const char *lpFile) = 0;
    
    virtual int32_t OpenFromBuffer(const char *lpBuffer) = 0;
 
    virtual int32_t AddNull(const char *lpKey) = 0;

    virtual int32_t AddBool(const char *lpKey, bool bValue) = 0;

    virtual int32_t AddInt(const char *lpKey, int64_t nValue) = 0;

    virtual int32_t AddDouble(const char *lpKey, double dValue) = 0;

    virtual int32_t AddString(const char *lpKey, const char *lpValue) = 0;

    virtual IJsonObj *AddArray(const char *lpKey) = 0;

    virtual IJsonObj *AddObject(const char *lpKey) = 0;

    virtual bool GetNull(const char *lpKey) = 0;

    virtual bool GetBool(const char *lpKey, bool bDefaultValue = false) = 0;

    virtual int64_t GetInt(const char *lpKey, int64_t nDefaultValue = 0) = 0;

    virtual double GetDouble(const char *lpKey, double dDefaultValue = 0.0) = 0;

    virtual const char *GetString(const char *lpKey, const char *lpDefaultValue = nullptr) = 0;

    virtual IJsonObj *GetArray(const char *lpKey) = 0;

    virtual IJsonObj *GetObject(const char *lpKey) = 0;

    virtual bool IsExist(const char *lpKey) = 0;

    virtual ObjType GetType() = 0;

    virtual uint32_t GetSize() = 0;

    virtual ObjType GetType(const char *lpKey) = 0;

    virtual ObjType GetType(uint32_t uIndex) = 0;

    virtual int32_t GetItem(uint32_t uIndex, KvItem *lpKvItem) = 0;

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