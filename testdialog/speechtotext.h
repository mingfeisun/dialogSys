#ifndef SPEECHTOTEXT_H
#define SPEECHTOTEXT_H
#include <iostream>
#include <string>

using std::string;

class speechToText
{
public:
  char* rec_result;

  speechToText();
  ~speechToText();
  bool startRecog();
  void setFileName(char* newFileName);

private:
  int ret;
  char* fileName;
  const char* session_begin_params;
  const char* login_params;

  bool run_iat(char* audio_file, const char* session_begin_params);
};

#endif // SPEECHTOTEXT_H
