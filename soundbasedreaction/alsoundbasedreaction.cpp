#include "alsoundbasedreaction.h"
#include "exec_shell.h"

#include <alvalue/alvalue.h>
#include <alcommon/alproxy.h>
#include <alproxies/almemoryproxy.h>
#include <alcommon/albroker.h>
#include <iostream>
#include <fstream>

ALSoundBasedReaction::ALSoundBasedReaction(boost::shared_ptr<ALBroker> broker, const std::string& name)
   :ALSoundExtractor(broker, name)
{
  setModuleDescription("sudden and significative noise is heard.");

  functionName("saveSendData", getName(), "");
  BIND_METHOD(ALSoundBasedReaction::saveSendData);
}

void ALSoundBasedReaction::init()
{
  memProxy = new AL::ALMemoryProxy(getParentBroker());
  memProxy->declareEvent("sendRawData");

  fProxyToTextToSpeech.setLanguage("English");
  memProxy->subscribeToEvent("sendRawData", getName(), "saveSendData");

  audioDevice->callVoid("setClientPreferences", getName(), 16000, (int)FRONTCHANNEL, 0 );
  startDetection();
}

ALSoundBasedReaction::~ALSoundBasedReaction()
{
  stopDetection();
}

void ALSoundBasedReaction::process(const int & nbOfChannels,
                                   const int & nbrOfSamplesByChannel,
                                   const AL_SOUND_FORMAT * buffer,
                                   const ALValue & timeStamp)
{
  static int index = 0;
  if(index == 1365*10){
      index = 0;
      memProxy->raiseEvent("sendRawData", true);
  }
  else{
      for(int i=0; i<nbrOfSamplesByChannel; i++){
           rawData[index++] = buffer[i];
      }
  }
}

void ALSoundBasedReaction::saveSendData()
{
    FILE* wav = fopen("raw.dat", "w");
    fwrite(rawData, sizeof(int), 1365*10, wav);
    fclose(wav);
    sendToWitAI();
}

void ALSoundBasedReaction::sendToWitAI()
{
    char* cmd = "sh curl.sh";
    std::cout<<exec_shell(std::string(cmd))<<std::endl;
}
