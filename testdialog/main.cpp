#include <cstring>
#include <iostream>
#include <string>
#include "converter.h"
#include "wakeup.h"
#include "dialogtext.h"
//#include "exec_shell.h"

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

  while(1){
      if(wakeup->call<bool>("getStatus")){
          wakeup->callVoid("stopStandUp");
          dialogText testDialog;
          testDialog.init();
          AL::ALModule::createModule<Converter>(broker, "Converter");
          boost::shared_ptr<AL::ALProxy> conv = broker->getProxy("Converter");
          conv->callVoid("start");
          while(1){
              if(conv->call<bool>("getReady")){
                  if( conv->call<bool>("getExit") == true){
                      goto ext;
                  }
                  string result = conv->call<string>("getResult");
                  if( result != ""){
                      std::cout<<result<<std::endl;
                      string temp = testDialog.getResponse(result);
                      std::cout<<temp<<std::endl;
                      conv->callVoid("sayThis", temp);
                      conv->callVoid("flushResult");
                  }
                  conv->callVoid("start");
              }
          }
      }
  }
ext:return 0;
}
