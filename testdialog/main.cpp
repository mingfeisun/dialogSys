#include <iostream>
#include <string>
#include "texttospeech.h"

using std::string;
using std::cin;
using std::cout;

int main()
{
  const string ip = "192.168.1.102";
  const int port = 9559;
  string testStr;
  cout<<"The input:";
  cin>>testStr;
  textToSpeech tts(ip, port);
  tts.say(testStr);
  return 0;
}
