#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H

#include <iostream>
#include <string>
#include <alproxies/altexttospeechproxy.h>

using std::string;

class textToSpeech{
public:
    textToSpeech(boost::shared_ptr<AL::ALBroker> broker );

    virtual ~textToSpeech();
    bool say(const string str);

private:
    AL::ALTextToSpeechProxy* tts;
};

#endif // TEXTTOSPEECH_H
