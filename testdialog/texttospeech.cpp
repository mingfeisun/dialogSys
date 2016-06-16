#include "texttospeech.h"
#include <iostream>
#include <alerror/alerror.h>
#include <alproxies/altexttospeechproxy.h>

textToSpeech::textToSpeech(boost::shared_ptr<AL::ALBroker> broker )
    :tts(new AL::ALTextToSpeechProxy(broker))
{
    tts->setLanguage("Chinese");
}

textToSpeech::~textToSpeech()
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
