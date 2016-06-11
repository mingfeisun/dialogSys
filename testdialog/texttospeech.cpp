#include "texttospeech.h"
#include <iostream>
#include <alerror/alerror.h>
#include <alproxies/altexttospeechproxy.h>

textToSpeech::textToSpeech(std::string ip, int port)
    :tts(new AL::ALTextToSpeechProxy(ip, port))
{
}

textToSpeech::~textToSpeech()
{
}

void textToSpeech::init()
{
}

bool textToSpeech::say(const std::string str)
{
    try{
        tts->say(str);
        return true;
    }
    catch(const AL::ALError& e){
        std::cerr<<"Error:"<<e.what()<<std::endl;
        return false;
    }
}
