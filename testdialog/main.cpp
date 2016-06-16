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
  //string testStr = "你好，大家好";

  boost::shared_ptr<AL::ALBroker> broker = AL::ALBroker::createBroker("test", "", 0, ip, port);
  AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
  AL::ALBrokerManager::getInstance()->addBroker(broker);
  //AL::ALModule::createModule<speechToText>(broker, "speechToText");

  speechToText stt(broker, "test");
  //stt.init();
  //std::cout<<stt.isPCalled()<<std::endl;
  stt.test();

  std::cout<<stt.rec_result<<std::endl;
  textToSpeech tts(broker);
  tts.say(stt.rec_result);

  return 0;
}
