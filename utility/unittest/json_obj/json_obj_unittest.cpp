#include <gtest/gtest.h>
#include <json_obj.h>

TEST(JsonObj, SetAndGet)
{
    auto lpJsonObj = NewJsonObject();
    EXPECT_NE(lpJsonObj, nullptr);
    EXPECT_EQ(lpJsonObj->Init(cppbase::IJsonObj::ObjType::Object), 0);
    EXPECT_EQ(lpJsonObj->AddNull("null"), 0);
    EXPECT_EQ(lpJsonObj->AddBool("bool", true), 0);
    EXPECT_EQ(lpJsonObj->AddInt("int", 100), 0);
    EXPECT_EQ(lpJsonObj->AddString("string", "string"), 0);
    EXPECT_EQ(lpJsonObj->AddArray("array")->AddInt("arr1", 1), 0);
    EXPECT_EQ(lpJsonObj->AddObject("object")->AddString("str1", "str1"), 0);
    EXPECT_EQ(lpJsonObj->GetNull("null"), true);
    EXPECT_EQ(lpJsonObj->GetBool("bool", false), true);
    EXPECT_EQ(lpJsonObj->GetInt("int", 0), 100);
    EXPECT_EQ(strcmp(lpJsonObj->GetString("string", nullptr), "string"), 0) << lpJsonObj->GetString("string", nullptr);
    cppbase::IJsonObj::KvItem kvItem;
    EXPECT_EQ(lpJsonObj->GetArray("array")->GetItem(0, &kvItem), 0);
    EXPECT_EQ(kvItem.nValue, 1);
    EXPECT_EQ(lpJsonObj->GetObject("object")->GetString("str1", nullptr), "str1");
    EXPECT_EQ(lpJsonObj->GetSize(), 6);
    EXPECT_EQ(lpJsonObj->GetType(), cppbase::IJsonObj::ObjType::Object);
    DeleteJsonObject(lpJsonObj);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}