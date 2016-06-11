#include <iostream>
#include <string>
#include "texttospeech.h"
using std::string;

int main()
{
  string ip = "192.168.1.102";
  int port = 9559;
  string testStr="我是中国人";
  textToSpeech tts(ip, port);
  tts.say(testStr);
  return 0;
}
