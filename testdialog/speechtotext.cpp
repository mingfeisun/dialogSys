#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "speechtotext.h"
#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"

#define	BUFFER_SIZE	4096
#define FRAME_LEN	640
#define HINTS_SIZE  100
#define LOGIN_PARAMS "appid = 575c26c4, work_dir = ."
#define SESSION_PARAMS "sub = iat, domain = iat, language = zh_ch, accent = mandarin, \
sample_rate = 16000, result_type = plain, result_encoding = utf8"

speechtotext::speechtotext()
    :ret(MSP_SUCCESS),fileName(NULL),
login_params(LOGIN_PARAMS),session_begin_params(SESSION_PARAMS)

{
    rec_result[BUFFER_SIZE] = {NULL};
    ret = MSPLogin(NULL, NULL, login_params);
    if (MSP_SUCCESS != ret)
    {
        printf("MSPLogin failed , Error code %d.\n",ret);
        MSPLogout();
    }
}

speechtotext::~speechtotext()
{
    MSPLogout();
}

bool speechtotext::startRecog()
{
    if(fileName == NULL){
        return false;
    }
    return run_iat(fileName, session_begin_params);
}

void speechtotext::setFileName(const char *newFileName)
{
    strcpy(fileName, newFileName);
}

bool speechtotext::run_iat(const char *audio_file, const char *session_begin_params)
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
        usleep(200*1000);
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
        usleep(150*1000);
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
