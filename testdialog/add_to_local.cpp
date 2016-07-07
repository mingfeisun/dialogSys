#include "converter.h"
#include "wakeup.h"

#include <boost/shared_ptr.hpp>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>

#ifdef _WIN32
#define ALCALL __declspec(dllexport)
#else
#define ALCALL
#endif


extern "C"
{
    ALCALL int _createModule(boost::shared_ptr<AL::ALBroker> broker){
        AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
        AL::ALBrokerManager::getInstance()->addBroker(broker);
        AL::ALModule::creatModule<wakeUp>(broker,"wakeUp");
        AL::ALModule::creatModule<Converter>(broker,"Converter");
        return 0;
    }

    ALCALL int _closeModule(){
        return 0;
    }
}
