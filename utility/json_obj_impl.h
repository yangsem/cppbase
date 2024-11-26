#ifndef __JSON_OBJ_IMPL_H_
#define __JSON_OBJ_IMPL_H_

#include <os_common.h>
#include <json_obj.h>
#include <vector>
#include <unordered_map>

namespace cppbase
{

class CJsonObjImpl : public IJsonObj
{
    struct _ValueType
    {
        uint8_t Reserve[64];
    };

    using KeyType = std::string;
    using StringValueType = std::string;
    using ArrayValueType = std::vector<_ValueType>;
    using ObjValueType = std::unordered_map<KeyType, _ValueType>;

    union ValueType
    {
        bool bValue;
        int64_t nValue;
        std::string strValue;
        ArrayValueType arrValue;
        ObjValueType objValue;

        ValueType() {}
        ~ValueType() {}
    };

public:
    CJsonObjImpl();
    CJsonObjImpl(ObjType eType, const void *lpValue);
    ~CJsonObjImpl() override;

    int32_t Init(ObjType eType) override;

    int32_t OpenFromFile(const char *lpFile) override;
    int32_t OpenFromBuffer(const char *lpBuffer) override;

    int32_t Set(const char *lpKey, bool bValue) override;
    int32_t Set(const char *lpKey, int64_t nValue) override;
    int32_t Set(const char *lpKey, const char *lpValue) override;
    IJsonObj *Set(const char *lpKey, ObjType eType) override;

    bool Get(const char *lpKey, bool bDefaultValue = false) override;
    int64_t Get(const char *lpKey, int64_t nDefaultValue = 0) override;
    const char *Get(const char *lpKey, const char *lpDefaultValue = nullptr) override;
    IJsonObj *Get(const char *lpKey) override;

    ObjType GetType() override;
    uint32_t GetSize() override;
    ObjType GetType(const char *lpKey) override;
    ObjType GetType(uint32_t uIndex) override;
    int32_t GetKeyValue(uint32_t uIndex, KvItem *lpKvItem) override;

    const char *GetJsonStr(bool bPretty) override;

private:
    ObjType m_eType{ObjType::Object};
    ValueType m_unValue;
};

}

#endif //__JSON_OBJ_IMPL_H_
