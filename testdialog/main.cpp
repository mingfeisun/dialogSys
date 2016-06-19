#include <cstring>
#include <iostream>
#include <string>
#include "converter.h"
#include "wakeup.h"

#include <boost/shared_ptr.hpp>
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
  AL::ALModule::createModule<wakeUp>(broker, "wakeUp");

  boost::shared_ptr<AL::ALProxy> wakeup = broker->getProxy("wakeUp");

  //AL::ALModule::createModule<Converter>(broker, "Converter");
  //boost::shared_ptr<AL::ALProxy> conv = broker->getProxy("Converter");
  //conv->callVoid("test");

  while(1){
      if(wakeup->call<bool>("getStatus")){
          wakeup->callVoid("stopStandUp");
          AL::ALModule::createModule<Converter>(broker, "Converter");
          boost::shared_ptr<AL::ALProxy> conv = broker->getProxy("Converter");
          conv->callVoid("sayThis", "你好，请问有什么可以帮助您的么？");
          conv->callVoid("start");
          while(1){
              if(conv->call<bool>("getReady")){
                  string result = conv->call<string>("getResult");
                  if( result != ""){
                      std::cout<<result<<std::endl;
                      conv->callVoid("sayThis", result);
                      conv->callVoid("flushResult");
                      if( result == "谢谢"){
                          goto ext;
                      }
                  }
                  conv->callVoid("start");
              }
          }
      }
  }
ext:return 0;
}
