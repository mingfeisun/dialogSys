#ifndef TEXTTOSPEECH_H
#define TEXTTOSPEECH_H

#include <iostream>
#include <string>
#include <alcommon/alproxy.h>
#include <alproxies/altexttospeechproxy.h>

using std::string;

class textToSpeech{
private:
    boost::shared_ptr<AL::ALTextToSpeechProxy> tts;
    string tosay;

public:
    textToSpeech(string ip, int port);

    virtual ~textToSpeech();
    virtual void init();
    bool say(const string str);
};

#endif // TEXTTOSPEECH_H
