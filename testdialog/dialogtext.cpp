#include <iostream>
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
    string cmd = "curl http://qycpu6.cse.ust.hk:8080/home/dialogue_";
    return exec_shell(cmd+text+"/");
}
