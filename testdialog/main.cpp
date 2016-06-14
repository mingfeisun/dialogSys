#include <iostream>
#include <string>
#include "texttospeech.h"
#include "speechtotext.h"

#include <alcommon/albroker.h>
#include <alcommon/almodule.h>
#include <alcommon/albrokermanager.h>

using std::string;
using std::cin;
using std::cout;

int main()
{
  const string ip = "192.168.1.102";
  const int port = 9559;
  string testStr;

  boost::shared_ptr<AL::ALBroker> broker = AL::ALBroker::createBroker("test", "", 0, ip, port);
  AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
  AL::ALBrokerManager::getInstance()->addBroker(broker);

  speechToText stt(broker, "test");
  stt.speechDetecting();

  textToSpeech tts(ip, port);
  tts.say(testStr);

  return 0;
}
