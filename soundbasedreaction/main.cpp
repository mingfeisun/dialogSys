#include <boost/shared_ptr.hpp>
#include <alcommon/albroker.h>
#include <alcommon/almodule.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/altoolsmain.h>

#include "alsoundbasedreaction.h"


int main()
{
  boost::shared_ptr<AL::ALBroker> pBroker = AL::ALBroker::createBroker("test", "", 0, "192.168.1.102", 9559);
  AL::ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
  AL::ALBrokerManager::getInstance()->addBroker(pBroker);

  AL::ALModule::createModule<ALSoundBasedReaction>(pBroker,"ALSoundBasedReaction");
  while(1);

  return 0;
}
