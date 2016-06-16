#ifndef CATCHEVENTS_H
#define CATCHEVENTS_H
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <alcommon/almodule.h>
#include <alproxies/almemoryproxy.h>

using std::string;

namespace AL {
    class ALBroker;
}


class catchEvents : public AL::ALModule
{
public:
  catchEvents(boost::shared_ptr<AL::ALBroker> broker, const string& name);
  virtual ~catchEvents();

  virtual void init();
  void callback(const std::string& key, const AL::ALValue& val, const AL::ALValue& msg);

private:
  AL::ALMemoryProxy memPro;
};

#endif // CATCHEVENTS_H
