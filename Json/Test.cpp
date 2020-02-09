#include "Json/Test.h"
#include "Core/Logger.h"

static String GetJsonString()
{
    Json::JsonWriter writer(true);
    writer.Name(CT_TEXT("name")).Value(CT_TEXT("Tom"));
    writer.Name(CT_TEXT("age")).Value(20);
    writer.Name(CT_TEXT("male")).Value(true);
    writer.Name(CT_TEXT("interests")).PushArray().Value(CT_TEXT("music")).Value(CT_TEXT("reading")).Pop();
    writer.Name(CT_TEXT("score")).PushObject().Name(CT_TEXT("math")).Value(90).Name(CT_TEXT("science")).Value(95.5).Pop();
    writer.Name(CT_TEXT("extra")).Value(nullptr);
    writer.Name(CT_TEXT("earray")).PushArray().Pop();
    writer.Name(CT_TEXT("eobject")).PushObject();

    String str;
    writer.Write(str);
    return str;
}

static void DumpJson()
{
    String str = GetJsonString();
    CT_LOG(Info, str);
    CT_LOG(Info, CT_TEXT("---------------------------"));

    Json::JsonReader reader;
    auto json = reader.Parse(str);

    for(auto child = json->child; child != nullptr; child = child->next)
    {
        CT_LOG(Info, CT_TEXT("name: {0}, type: {1}"), child->name, static_cast<int32>(child->type));
    }
}

void Json::Test()
{
    //String str0 = CT_TEXT("AABBCCAABBCCAABBCCAABBCC");

    //String str1 = str0.Replace(CT_TEXT("A"), CT_TEXT("1"));
    //String str2 = str0.ReplaceAll(CT_TEXT("BB"), CT_TEXT("2kkkkkk"));

    //logger.Info(CT_TEXT("{0} , {1}"), str1, str2);

    // String str0 = CT_TEXT("12335    ");

    // String str1 = str0.ReplaceAll(CT_TEXT(" "), CT_TEXT("*"));
    // String str2 = str0.TrimStart();
    // String str3 = str0.TrimEnd();
    // String str4 = str0.Trim();

    // logger.Info(CT_TEXT("{0},{1},{2},{3},"), str1, str2, str3, str4);

    DumpJson();
}