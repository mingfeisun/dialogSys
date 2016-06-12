#ifndef SPEECHTOTEXT_H
#define SPEECHTOTEXT_H
#include <iostream>
#include <string>

using std::string;

class speechtotext
{
public:
  char rec_result[];

  speechtotext();
  ~speechtotext();
  bool startRecog();
  void setFileName(const char* newFileName);

private:
  int rec;
  const char* fileName;
  const char* session_begin_params;
  const char* login_params;

  bool run_iat(const char* audio_file, const char* session_begin_params);
};

#endif // SPEECHTOTEXT_H
