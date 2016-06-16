#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#include <iostream>

#include "converter.h"
#include <iflytek/qisr.h>
#include <iflytek/msp_cmn.h>
#include <iflytek/msp_errors.h>

#include <qi/os.hpp>
#include <qi/log.hpp>
#include <alcommon/albroker.h>
#include <alcommon/almodule.h>
#include <alproxies/alaudiodeviceproxy.h>
#include <alproxies/altexttospeechproxy.h>
#include <alproxies/alaudiorecorderproxy.h>

#define	BUFFER_SIZE	4096
#define FRAME_LEN	640
#define HINTS_SIZE  100
#define FILENAME_SIZE  100

const char* WAV_NAME_LOCAL = "/home/nao/audio/test.wav";
const char* WAV_NAME_REMOTE = "/home/mingfeisun/Desktop/test.wav";

using namespace AL;

Converter::Converter(boost::shared_ptr<AL::ALBroker> broker, const string& name)
    :AL::ALModule(broker, name), tts(new AL::ALTextToSpeechProxy(broker))

{
    rec_result = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    strcpy(rec_result, "hello");

    rec_now = false;
    tts->setLanguage("Chinese");

    functionName("run_iat", getName(), "");
    addParam("audio_file", "audioFile");
    addParam("session_begin_params", "params");
    setReturn("bool", "result boolean value");
    BIND_METHOD(Converter::run_iat);

    recogInit();
    proxyInit(broker);
}

Converter::~Converter()
{
    MSPLogout();
}

void Converter::init()
{
}

void Converter::recogInit()
{
    login_params = "appid = 575c26c4, work_dir = .";
    session_begin_params = "sub = iat, domain = iat, language = zh_ch, accent = mandarin, "
                         "sample_rate = 16000, result_type = plain, result_encoding = utf8";
    ret = MSP_SUCCESS;

    ret = MSPLogin(NULL, NULL, login_params);
    if (MSP_SUCCESS != ret)
    {
        printf("MSPLogin failed , Error code %d.\n",ret);
        MSPLogout();
    }
}

void Converter::proxyInit(boost::shared_ptr<AL::ALBroker> broker)
{
    audio_rec_pro = new AL::ALAudioRecorderProxy(broker);
    audio_dev_pro = new AL::ALAudioDeviceProxy(broker);
    audio_dev_pro->enableEnergyComputation();
}

void Converter::sayThis(string tosay)
{
    audio_dev_pro->disableEnergyComputation();
    tts->say(tosay);
    audio_dev_pro->enableEnergyComputation();
}

void Converter::speechDetecting()
{
    const float ENERGY_TH = 800; // [0, 32768]
    float energy_level = audio_dev_pro->getFrontMicEnergy();
    printf("speech energy: %f\n", energy_level);
    if(energy_level > ENERGY_TH && !rec_now){
        recordingStop(false);
    }
    if(energy_level > ENERGY_TH && rec_now){
        qi::os::sleep(1);
    }
    if(energy_level < ENERGY_TH && rec_now){
        qi::os::sleep(1);
        if(energy_level < ENERGY_TH && rec_now){
            recordingStop(true);
            run_iat(WAV_NAME_REMOTE, session_begin_params);
        }
    }
}

void Converter::recordingStop(bool stop)
{
    if(stop){
      audio_rec_pro->stopMicrophonesRecording();
      rec_now = false;
      system("scp nao@192.168.1.102:~/audio/test.wav /home/mingfeisun/Desktop/test.wav ");
    }
    else{
      AL::ALValue channels;
      channels.arrayPush(0); //left
      channels.arrayPush(0); //right
      channels.arrayPush(1); //front
      channels.arrayPush(0); //reat
      audio_rec_pro->startMicrophonesRecording(WAV_NAME_LOCAL, "wav", 16000, channels);
      qi::os::sleep(1);
      rec_now = true;
    }
}

bool Converter::run_iat(const char *audio_file, const char *session_begin_params)
{
    bool flag = true;

    const char*		session_id					=	NULL;
    char			hints[HINTS_SIZE]			=	{NULL};
    unsigned int	total_len					=	0;
    int				aud_stat					=	MSP_AUDIO_SAMPLE_CONTINUE ;
    int				ep_stat						=	MSP_EP_LOOKING_FOR_SPEECH;
    int				rec_stat					=	MSP_REC_STATUS_SUCCESS ;
    int				errcode						=	MSP_SUCCESS ;

    FILE*			f_pcm						=	NULL;
    char*			p_pcm						=	NULL;
    long			pcm_count					=	0;
    long			pcm_size					=	0;
    long			read_size					=	0;

    if (NULL == audio_file){
        flag = false;
        goto iat_exit;
    }

    f_pcm = fopen(audio_file, "rb");
    if (NULL == f_pcm)
    {
        printf("\nopen [%s] failed! \n", audio_file);
        flag = false;
        goto iat_exit;
    }

    fseek(f_pcm, 0, SEEK_END);
    pcm_size = ftell(f_pcm);
    fseek(f_pcm, 0, SEEK_SET);

    p_pcm = (char *)malloc(pcm_size);
    if (NULL == p_pcm)
    {
        printf("\nout of memory! \n");
        flag = false;
        goto iat_exit;
    }

    read_size = fread((void *)p_pcm, 1, pcm_size, f_pcm);
    if (read_size != pcm_size)
    {
        printf("\nread [%s] error!\n", audio_file);
        flag = false;
        goto iat_exit;
    }

    session_id = QISRSessionBegin(NULL, session_begin_params, &errcode);
    if (MSP_SUCCESS != errcode)
    {
        printf("\nQISRSessionBegin failed! error code:%d\n", errcode);
        flag = false;
        goto iat_exit;
    }

    while (1)
    {
        unsigned int len = 10 * FRAME_LEN;
        int ret = 0;

        if (pcm_size < 2 * len)
            len = pcm_size;
        if (len <= 0)
            break;

        aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;
        if (0 == pcm_count)
            aud_stat = MSP_AUDIO_SAMPLE_FIRST;

        ret = QISRAudioWrite(session_id, (const void *)&p_pcm[pcm_count], len, aud_stat, &ep_stat, &rec_stat);
        printf(">");
        if (MSP_SUCCESS != ret)
        {
            printf("\nQISRAudioWrite failed! error code:%d\n", ret);
            flag = false;
            goto iat_exit;
        }

        pcm_count += (long)len;
        pcm_size  -= (long)len;

        if (MSP_REC_STATUS_SUCCESS == rec_stat)
        {
            const char *rslt = QISRGetResult(session_id, &rec_stat, 0, &errcode);
            if (MSP_SUCCESS != errcode)
            {
                printf("\nQISRGetResult failed! error code: %d\n", errcode);
                flag = false;
                goto iat_exit;
            }
            if (NULL != rslt)
            {
                unsigned int rslt_len = strlen(rslt);
                total_len += rslt_len;
                if (total_len >= BUFFER_SIZE)
                {
                    printf("\nno enough buffer for rec_result !\n");
                    flag = false;
                    goto iat_exit;
                }
                strncat(rec_result, rslt, rslt_len);
            }
        }

        if (MSP_EP_AFTER_SPEECH == ep_stat)
            break;
        //usleep(1*1000);
    }
    errcode = QISRAudioWrite(session_id, NULL, 0, MSP_AUDIO_SAMPLE_LAST, &ep_stat, &rec_stat);
    if (MSP_SUCCESS != errcode)
    {
        printf("\nQISRAudioWrite failed! error code:%d \n", errcode);
        flag = false;
        goto iat_exit;
    }

    while (MSP_REC_STATUS_COMPLETE != rec_stat)
    {
        const char *rslt = QISRGetResult(session_id, &rec_stat, 0, &errcode);
        if (MSP_SUCCESS != errcode)
        {
            printf("\nQISRGetResult failed, error code: %d\n", errcode);
            flag = false;
            goto iat_exit;
        }
        if (NULL != rslt)
        {
            unsigned int rslt_len = strlen(rslt);
            total_len += rslt_len;
            if (total_len >= BUFFER_SIZE)
            {
                printf("\nno enough buffer for rec_result !\n");
                flag = false;
                goto iat_exit;
            }
            strncat(rec_result, rslt, rslt_len);
        }
        //usleep(150*1000);
    }

iat_exit:
    if (NULL != f_pcm)
    {
        fclose(f_pcm);
        f_pcm = NULL;
    }
    if (NULL != p_pcm)
    {	free(p_pcm);
        p_pcm = NULL;
    }

    QISRSessionEnd(session_id, hints);
    return flag;
}
