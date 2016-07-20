#include <cstring>
#include <iostream>
#include <string>
#include "converter.h"
#include "wakeup.h"
#include "dialogtext.h"
#include "tulingmodule.h"
//#include "exec_shell.h"

#include <qi/log.hpp>
#include <boost/shared_ptr.hpp>
#include <alproxies/almotionproxy.h>
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

  AL::ALMotionProxy* motion = new AL::ALMotionProxy(broker);

  tulingModule tuLing;
  dialogText testDialog;

  motion->rest();

  while(1){
      if(wakeup->call<bool>("getStatus")){
          testDialog.init();
          wakeup->callVoid("stopStandUp");

          static boost::shared_ptr<AL::ALModule> conMod = AL::ALModule::createModule<Converter>(broker, "Converter");
          boost::shared_ptr<AL::ALProxy> conv = broker->getProxy("Converter");
          conv->callVoid("proxyInit");

          motion->wakeUp();

          tuLing.init();

          conv->callVoid("start");
          while(1){
              if(conv->call<bool>("getReady")){
                  string result = conv->call<string>("getResult");
                  qiLogInfo("SPR Result Get:")<<result<<std::endl;
                  string temp;
                  if(conv->call<bool>("getCafe")){
                      temp = testDialog.getResponse(result);
                  }
                  else{
                      temp = tuLing.getResponse(result);
                  }
                  qiLogInfo("Dialog Result Get:")<<temp<<std::endl;
                  conv->callVoid("sayThis", temp);
                  if( conv->call<bool>("getExit") == true){
                      wakeup->callVoid("standUp");
                      conv->callVoid("offCafe");
                      conMod->exit();
                      break;
                  }
                  conv->callVoid("flushResult");
                  conv->callVoid("start");
              }
          }
      }
      motion->post.rest();
  }
  return 0;
}
