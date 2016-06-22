#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <memory>
#include <string>

#include <iostream>

#include "converter.h"
#include "exec_shell.h"

#include <qi/os.hpp>
#include <qi/log.hpp>
#include <alcommon/almodule.h>
#include <alproxies/almemoryproxy.h>
#include <alproxies/altexttospeechproxy.h>
#include <alproxies/alaudiorecorderproxy.h>
#include <alproxies/alspeechrecognitionproxy.h>

const char* WAV_NAME_LOCAL = "/home/nao/audio/test.wav";

using namespace AL;

Converter::Converter(boost::shared_ptr<ALBroker> broker, const std::string &name)
    :AL::ALModule(broker, name), tts(new AL::ALTextToSpeechProxy(broker))

{
    flushResult();

    tts->setLanguage("Chinese");

    functionName("getResult", getName(), "");
    setReturn("string", "string");
    BIND_METHOD(Converter::getResult);

    functionName("flushResult", getName(), "");
    BIND_METHOD(Converter::flushResult);

    functionName("speechDetecting", getName(), "");
    BIND_METHOD(Converter::speechDetecting);

    functionName("start", getName(), "");
    BIND_METHOD(Converter::start);

    functionName("getReady", getName(), "");
    BIND_METHOD(Converter::getReady);

    functionName("test", getName(), "");
    BIND_METHOD(Converter::test);

    functionName("sayThis", getName(), "");
    addParam("tosay", "tosay");
    BIND_METHOD(Converter::sayThis);

    ready = true;
}

Converter::~Converter()
{
    mem_pro->unsubscribeToEvent("ALSpeechRecognition/Status", getName());
}

void Converter::init()
{
    proxyInit();
}

void Converter::proxyInit()
{
    mem_pro = new AL::ALMemoryProxy(getParentBroker());
    audio_rec_pro = new AL::ALAudioRecorderProxy(getParentBroker());
    speech_recog_pro = new AL::ALSpeechRecognitionProxy(getParentBroker());

    std::vector<std::string> wordList;
    wordList.push_back("谢谢");

    try{
        speech_recog_pro->setLanguage("Chinese");
        speech_recog_pro->setAudioExpression(true);
        speech_recog_pro->setVisualExpression(true);
        speech_recog_pro->setVocabulary(wordList, false);
    }
    catch(AL::ALError& e){
        //std::cout<<e.what()<<std::endl;
    }

}

void Converter::sayThis(string tosay)
{
    if(ready){
        tts->say(tosay);
    }
}

void Converter::speechDetecting(std::string eventName, AL::ALValue status, std::string subId)
{
    std::cout<<(string)status<<"  ";
    if(std::string(status) == "ListenOn" && !rec_now ){
        startRecording();
    }
    if((std::string(status) == "ListenOff") && rec_now ){
        stopRecording();
        mem_pro->unsubscribeToEvent("ALSpeechRecognition/Status", getName());
        speech_recog_pro->pause(true);
        tts->say("好的，您稍等，我确认下。");
        witAI();
    }
}

void Converter::startRecording()
{
    AL::ALValue channels;
    channels.arrayPush(0); //left
    channels.arrayPush(0); //right
    channels.arrayPush(1); //front
    channels.arrayPush(0); //reat
    try{
        audio_rec_pro->startMicrophonesRecording(WAV_NAME_LOCAL, "wav", 16000, channels);
    }
    catch(AL::ALError& e){
        std::cout<<e.what()<<std::endl;
    }

    rec_now = true;
}

void Converter::stopRecording()
{
    audio_rec_pro->stopMicrophonesRecording();
    rec_now = false;
}

void Converter::flushResult()
{
    rec_result = "";
}

void Converter::start()
{
    mem_pro->subscribeToEvent("ALSpeechRecognition/Status", getName(), "speechDetecting");
    speech_recog_pro->pause(false);
    ready = false;
}

bool Converter::getReady()
{
    return ready;
}

string Converter::getResult()
{
    return rec_result;
}

void Converter::test()
{
    witAI();
}

bool Converter::witAI()
{

    char* cmd = "ssh nao@192.168.1.102 'bash -s' < upload.sh";

    std::string result = exec_shell(cmd);

    std::cout<<result<<std::endl;
    std::string loc = "\"_text\"";
    int ind_e = result.find(loc);
    while(result[ind_e] != ':'){
        ind_e++;
    }
    while(result[ind_e] != '"'){
        if(result[ind_e] == ','){
            rec_result = "";
            return true;
        }
        ind_e++;
    }
    int ind_s = ++ind_e;
    while(result[ind_e] != '"'){
        ind_e++;
    }
    rec_result = result.substr(ind_s, ind_e-ind_s);

    ready = true;

    return true;
}
