#include "Json/Test.h"
#include "Core/Log.h"

static Log logger = Log(CT_TEXT("Json"));

void Json::Test()
{
    //String str0 = CT_TEXT("AABBCCAABBCCAABBCCAABBCC");

    //String str1 = str0.Replace(CT_TEXT("A"), CT_TEXT("1"));
    //String str2 = str0.ReplaceAll(CT_TEXT("BB"), CT_TEXT("2kkkkkk"));

    //logger.Info(CT_TEXT("{0} , {1}"), str1, str2);

    String str0 = CT_TEXT("12335    ");

    String str1 = str0.ReplaceAll(CT_TEXT(" "), CT_TEXT("*"));
    String str2 = str0.TrimStart();
    String str3 = str0.TrimEnd();
    String str4 = str0.Trim();

    logger.Info(CT_TEXT("{0},{1},{2},{3},"), str1, str2, str3, str4);
}