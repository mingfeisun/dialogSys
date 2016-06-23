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
    string cmd = "curl http://qycpu6.cse.ust.hk:8080/home/dialogue_";
    string text_res = exec_shell(cmd+text+"/");

    //char* expr = "animations/Stand/BodyTalk/BodyTalk_";
    char* expr = "animations/Stand/Gestures/Explain_";

    char buffer[200];
    sprintf(buffer, "^start(%s%d)%s", expr, index%12, text_res.c_str());
    index++;

    return string(buffer);
}
