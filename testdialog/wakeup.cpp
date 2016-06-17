#include "wakeup.h"
#include <iostream>

#include <alvalue/alvalue.h>
#include <alcommon/almodule.h>
#include <alcommon/albroker.h>

using std::string;
using std::vector;

wakeUp::wakeUp(boost::shared_ptr<AL::ALBroker> broker, const string name)
    :AL::ALModule(broker, name)
{
    wake_up_command = "hey Nao robot";

    setModuleDescription("");

    functionName("standUp", getName(), "standUp");
    BIND_METHOD(wakeUp::standUp);

    functionName("stopStandUp", getName(), "stopStandUp");
    BIND_METHOD(wakeUp::stopStandUp);

    functionName("getStatus", getName(), "getStatus");
    setReturn("bool", "bool");
    BIND_METHOD(wakeUp::getStatus);
}

wakeUp::~wakeUp()
{
}

void wakeUp::init()
{
    //standUp();
}

void wakeUp::standUp()
{
    mem = getParentBroker()->getMemoryProxy();
    speech_recog = getParentBroker()->getProxy("ALSpeechRecognition");
    speech_recog->callVoid("setLanguage", string("English"));
    speech_recog->callVoid("setAudioExpression", true);
    speech_recog->callVoid("setVisualExpression", true);

    vector<string> command_list;
    command_list.push_back(wake_up_command);
    speech_recog->callVoid("setVocabulary", command_list, false);

    mem->subscribeToEvent("WordRecognized", getName(), "onWakeUp");
}

void wakeUp::stopStandUp()
{
    mem->unsubscribeToEvent("WordRecognized", getName());
}

void wakeUp::onWakeUp(const string &name, const AL::ALValue &val, const string &myName)
{
    if((string)val[0] == wake_up_command && (float)val[1] >= 0.15){
        setStatus(true);
    }
    else{
        setStatus(false);
    }
}

bool wakeUp::getStatus()
{
    return status;
}

void wakeUp::setStatus(bool val)
{
    status = val;
}
