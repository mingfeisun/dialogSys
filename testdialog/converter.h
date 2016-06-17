#ifndef CONVERTER_H
#define CONVERTER_H
#include <iostream>
#include <string>

#include <alcommon/albroker.h>
#include <alcommon/almodule.h>
#include <boost/shared_ptr.hpp>
#include <alproxies/alaudiodeviceproxy.h>
#include <alproxies/altexttospeechproxy.h>
#include <alproxies/alaudiorecorderproxy.h>
#include <althread/almutex.h>
#include <qi/os.hpp>

using std::string;

namespace AL {
   class ALBroker;
}

class Converter : public AL::ALModule
{
public:
  Converter(boost::shared_ptr<AL::ALBroker> broker, const std::string &name);
  virtual ~Converter();
  virtual void init();
  void recogInit();
  void proxyInit();
  void sayThis(string tosay);
  void speechDetecting();
  void recordingStop(bool stop);
  void flushResult();
  string getResult();

private:
  int ret;
  bool rec_now;
  char* rec_result;
  char* session_begin_params;
  char* login_params;
  AL::ALTextToSpeechProxy* tts;
  AL::ALAudioDeviceProxy* audio_dev_pro;
  AL::ALAudioRecorderProxy* audio_rec_pro;

  bool run_iat(const char* audio_file, const char* session_begin_params);
};

#endif
