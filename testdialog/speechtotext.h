#ifndef SPEECHTOTEXT_H
#define SPEECHTOTEXT_H
#include <iostream>
#include <string>

#include <alcommon/almodule.h>
#include <boost/shared_ptr.hpp>
#include <alproxies/almemoryproxy.h>
#include <alproxies/alaudiodeviceproxy.h>
#include <alproxies/alaudiorecorderproxy.h>
#include <alproxies/alsounddetectionproxy.h>
#include <qi/os.hpp>

using std::string;

namespace AL {
   class ALBroker;
}

class speechToText : public AL::ALModule
{
public:
  char* rec_result;

  speechToText(boost::shared_ptr<AL::ALBroker> broker, const string& name);
  virtual ~speechToText();
  virtual void init();
  void recogInit();
  void proxyInit();
  void speechDetecting();
  void recording(bool stop);
  bool startRecog(const string& key, const AL::ALValue& val, const AL::ALValue& msg);
  void setFileName(char* newFileName);

private:
  int ret;
  bool rec_now;
  char* fileName;
  char* session_begin_params;
  char* login_params;
  AL::ALMemoryProxy* memory_pro;
  AL::ALAudioDeviceProxy* audio_dev_pro;
  AL::ALAudioRecorderProxy* audio_rec_pro;

  bool run_iat(char* audio_file, const char* session_begin_params);
};

#endif // SPEECHTOTEXT_H
