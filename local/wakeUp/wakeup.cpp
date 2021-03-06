#include "wakeup.h"
#include <iostream>

#include <qi/log.hpp>
#include <boost/shared_ptr.hpp>
#include <alvalue/alvalue.h>
#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alproxies/altexttospeechproxy.h>

#define COMMAND "Hey, Nao robot"

using std::string;
using std::vector;
using std::cout;
using std::endl;

wakeUp::wakeUp(boost::shared_ptr<AL::ALBroker> broker, const string name)
    :AL::ALModule(broker, name)
    ,wake_up_command(COMMAND)
    ,status(false)
{
    setModuleDescription("Wake Up for Dialog System");

    functionName("standUp", getName(), "standUp");
    BIND_METHOD(wakeUp::standUp);

    functionName("stopStandUp", getName(), "stopStandUp");
    BIND_METHOD(wakeUp::stopStandUp);

    functionName("getStatus", getName(), "getStatus");
    setReturn("bool", "bool");
    BIND_METHOD(wakeUp::getStatus);

    functionName("onWakeUp", getName(), "onWakeUp");
    BIND_METHOD(wakeUp::onWakeUp);
}

wakeUp::~wakeUp()
{
}

void wakeUp::init()
{
    qiLogInfo("Wake Up")<<"Wake Up module init"<<std::endl;
}

void wakeUp::standUp()
{
    mem          = getParentBroker()->getMemoryProxy();
    speech_recog = getParentBroker()->getProxy("ALSpeechRecognition");
    speak_out    = getParentBroker()->getProxy("ALTextToSpeech");

    try{
        speech_recog->callVoid("setLanguage", string("English"));
        speech_recog->callVoid("setAudioExpression", false);
        speech_recog->callVoid("setVisualExpression", false);

        speak_out->callVoid("setLanguage", string("Chinese"));

        vector<string> command_list;
        command_list.push_back(wake_up_command);
        speech_recog->callVoid("setVocabulary", command_list, false);
    }
    catch(const AL::ALError& e ){
        //cout<<e.what()<<endl;
        speech_recog->callVoid("pause", true);
        speech_recog->callVoid("setLanguage", string("English"));
        speech_recog->callVoid("setAudioExpression", false);
        speech_recog->callVoid("setVisualExpression", false);

        vector<string> command_list;
        command_list.push_back(wake_up_command);
        speech_recog->callVoid("setVocabulary", command_list, false);
        speech_recog->callVoid("pause", false);
    }
    mem->subscribeToEvent("WordRecognized", getName(), "onWakeUp");
}

void wakeUp::stopStandUp()
{
    mem->unsubscribeToEvent("WordRecognized", getName());
    speech_recog->callVoid("pause", true);
}

void wakeUp::onWakeUp(const string &name, const AL::ALValue &val, const string &myName)
{
    cout<<">";
    if((string)val[0] == wake_up_command && (float)val[1] >= 0.30){
        setStatus(true);
        speak_out->callVoid("say", "你好，请问有什么可以帮助您的么？");
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
