#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "dialogtext.h"
#include "exec_shell.h"

dialogText::dialogText()
{
}

void dialogText::init()
{
    getResponse("RESET");
}

string dialogText::getResponse(string text)
{
    static int index=0;
    if(text == ""){
        return string("不好意思，我没有听清楚你说的话^startTag(not know)，您可以再说一遍么？");
    }
    string cmd = "curl http://qycpu7.cse.ust.hk:8080/home/dialogue_";
    string text_res = exec_shell(cmd+text+'/');

    //char* expr = "animations/Stand/BodyTalk/BodyTalk_";
    char* expr = "animations/Stand/Gestures/Explain_";

    char buffer[200];
    sprintf(buffer, "^start(%s%d)%s", expr, index%12, text_res.c_str());
    index++;

    return string(buffer);
}
