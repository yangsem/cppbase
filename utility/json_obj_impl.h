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
        double dValue;
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

    int32_t AddNull(const char *lpKey) override;
    int32_t AddBool(const char *lpKey, bool bValue) override;
    int32_t AddInt(const char *lpKey, int64_t nValue) override;
    int32_t AddDouble(const char *lpKey, double dValue) override;
    int32_t AddString(const char *lpKey, const char *lpValue) override;
    IJsonObj *AddArray(const char *lpKey) override;
    IJsonObj *AddObject(const char *lpKey) override;

    bool GetNull(const char *lpKey) override;
    bool GetBool(const char *lpKey, bool bDefaultValue = false) override;
    int64_t GetInt(const char *lpKey, int64_t nDefaultValue = 0) override;
    double GetDouble(const char *lpKey, double dDefaultValue = 0.0) override;
    const char *GetString(const char *lpKey, const char *lpDefaultValue = nullptr) override;
    IJsonObj *GetArray(const char *lpKey) override;
    IJsonObj *GetObject(const char *lpKey) override;

    bool IsExist(const char *lpKey) override;
    
    ObjType GetType() override;
    uint32_t GetSize() override;
    ObjType GetType(const char *lpKey) override;
    ObjType GetType(uint32_t uIndex) override;
    int32_t GetItem(uint32_t uIndex, KvItem *lpKvItem) override;

    const char *GetJsonStr(bool bPretty) override;

private:
    bool IsNullChar(char ch);
    bool IsNumChar(char ch);
    bool IsCharZero2Nine(char ch);
    const char *ParseString(const char *lpContent, uint64_t &uIndex);
    const char *ParseNumber(const char *lpContent, uint64_t &uIndex, bool &bDouble);
    int32_t ParseArrNull(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj);
    int32_t ParseArrBool(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj);
    int32_t ParseArrNumber(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj);
    int32_t ParseArrString(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj);
    int32_t ParseArrNumber(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj);
    int32_t ParseKeyValue(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj);
    int32_t ParseObject(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj);
    int32_t ParseArray(const char *lpContent, uint64_t &uIndex, IJsonObj *lpJsonObj);
    int32_t ParseContent(const char *lpContent);

private:
    ObjType m_eType{ObjType::Unknow};
    ValueType m_unValue;
};

}

#endif //__JSON_OBJ_IMPL_H_
