#include <cstring>
#include <iostream>
#include <string>
#include "converter.h"

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

  boost::shared_ptr<AL::ALBroker> broker = AL::ALBroker::createBroker("test", "", 0, ip, port);
  AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
  AL::ALBrokerManager::getInstance()->addBroker(broker);

  Converter conv(broker, "test");
  while(1){
    conv.speechDetecting();
    std::cout<<conv.rec_result<<std::endl;
    conv.sayThis(string(conv.rec_result));
    strcpy(conv.rec_result, "");
  }
  return 0;
}
