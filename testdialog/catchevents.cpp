#include "catchevents.h"

#include <alvalue/alvaue.h>
#include <alcommon/alproxy.h>
#include <alcommon/albroker.h>

#include <qi/log.hpp>

catchEvents::catchEvents(boost::shared_ptr<AL::ALBroker> broker, const string &name)
    :AL::ALModule(broker, name), memPro(getParentBroker())
{
    setModuleDescription("");
    functionName("callback", getName(), "");
    BIND_METHOD(catchEvents::callback);
}

catchEvents::~catchEvents()
{
}

void catchEvents::init()
{
}

void catchEvents::callback(const std::string &key, const AL::ALValue &val, const AL::ALValue &msg)
{
}
