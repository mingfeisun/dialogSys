#ifndef SOUNDBASEDREACTION_H
#define SOUNDBASEDREACTION_H

#include <boost/shared_ptr.hpp>
#include <alcommon/almodule.h>
#include <string>
#include <iostream>
#include <alproxies/altexttospeechproxy.h>
#include <alproxies/almemoryproxy.h>
#include "alaudio/alsoundextractor.h"

using namespace AL;

class ALSoundBasedReaction : public ALSoundExtractor
{
public:

  ALSoundBasedReaction(boost::shared_ptr<ALBroker> broker,
                       const std::string & name);
  virtual ~ALSoundBasedReaction();

  void init();

  void process(const int & nbOfChannels,
               const int & nbrOfSamplesByChannel,
               const AL_SOUND_FORMAT * buffer,
               const ALValue & timeStamp);

private:
  ALTextToSpeechProxy fProxyToTextToSpeech;
  ALMemoryProxy* memProxy;

  int rawData[1365*10];

  void saveSendData();
  void sendToWitAI();
};

#endif  // SOUNDBASEDREACTION_H

