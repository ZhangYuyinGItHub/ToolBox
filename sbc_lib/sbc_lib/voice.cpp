// voice.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include <stdlib.h> 
#include <string.h>
//#include "adpcm.h"
#include "wav.h"
#include "voice.h"


unsigned char WaveHeader[] = {0x52,0x49,0x46,0x46,0x24,0xa5,0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
                              0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x80,0x3e,0x00,0x00,0x00,0x7d,0x00,0x00,
                              0x02,0x00,0x10,0x00,0x64,0x61,0x74,0x61,0x00,0xa5,0x00,0x00,0x0d,0x0a};

/*
 * @brief voice sbc decoder.
 */
int voice_sbc_decoder(char *input, char *output, T_SBC_PARAMS *p_encode_param)
{
    if ((input == NULL) || (input == ""))
        return RET_SRC_FILE_NOT_EXIST;

    if ((output == NULL) || (output == ""))
        return RET_DES_FILE_NOT_EXIST;

    if (p_encode_param == NULL)
        return RET_PARAM_ERROR;

    wav_pcm_header44* hwav = (wav_pcm_header44*)WaveHeader;;//wav file header

    int dataLength;//length of encode buffer, half of sample number
    int sample_num;//sample number
    int frame_size_after_enc = p_encode_param->bitpool * 2 + 8;

    unsigned char *EncodeBuffer;
    short *DecodeBuffer;

    int result = RET_CODE_SUCCESS;

    FILE *fpWav = NULL;//wav file handle

    fpWav = fopen(input, "rb");
    printf("读取语音信息:\n");
    if (fpWav)
    {
        fseek(fpWav,0,SEEK_END);
        dataLength = ftell (fpWav);
        sample_num = dataLength / frame_size_after_enc * VOICE_PCM_FRAME_SIZE / 2;//30 * 128;

        //update wav header info
        hwav->SubChun2Size = sample_num * 2;//16bit
        hwav->ChunkSize = hwav->SubChun2Size + frame_size_after_enc;//这个值在16k时是36
        hwav->ByteRate = 16000 * 2;//16k * 2

        EncodeBuffer = (unsigned char*)malloc(dataLength);
        memset(EncodeBuffer, 0, dataLength);
        fseek(fpWav,0,SEEK_SET);
        fread(EncodeBuffer, 1, dataLength, fpWav);
        fclose(fpWav);
        fpWav = NULL;
    }
    else
    {
        printf("Open wave file  failed!\n");
        result = RET_SRC_FILE_OPEN_FAILED;
    }

    // decode
    DecodeBuffer = (short*)malloc(sizeof(short) * sample_num);
    memset(DecodeBuffer, 0, sizeof(short) * sample_num);

    /*sbc init*/
    sbc_init_decoder();
    int out_len = VOICE_PCM_FRAME_SIZE;

    for (int index = 0; index < dataLength/frame_size_after_enc; index ++)
    {
        out_len = VOICE_PCM_FRAME_SIZE;
        result = sbc_decode(EncodeBuffer + index*frame_size_after_enc,
                            frame_size_after_enc,
                            (unsigned char *)(DecodeBuffer) + index * VOICE_PCM_FRAME_SIZE,
                            &out_len);
    }

    fpWav = fopen(output,"wb");
    if (fpWav)
    {
        //remove("encode.wav");
        fwrite(hwav,sizeof(wav_pcm_header44), 1,fpWav);
        fwrite(DecodeBuffer, sizeof(short), sample_num, fpWav);
        fclose(fpWav);
    }
    else
    {
        printf("open DecodedAudio error!");
        result = RET_DES_FILE_OPEN_FAILED;
    }


    if (EncodeBuffer != NULL)
        free(EncodeBuffer);
    else
        result = RET_SRC_BUFFER_MOC_FAILED;
    if (DecodeBuffer != NULL)
        free(DecodeBuffer);
    else
        result = RET_DES_BUFFER_MOC_FAILED;

    return result;
}

/**
*  @brief voice sbc encode
*/
int voice_sbc_encoder(char *input, char *output, T_SBC_PARAMS *p_encode_param)
{
    if ((input == NULL) || (input == ""))
        return RET_SRC_FILE_NOT_EXIST;

    if ((output == NULL) || (output == ""))
        return RET_DES_FILE_NOT_EXIST;

    if (p_encode_param == NULL)
        return RET_PARAM_ERROR;

    int dataLength;//length of encode buffer, half of sample number
    int sample_num;//sample number
    int frame_size_after_enc = p_encode_param->bitpool * 2 + 8;

    int result = RET_CODE_SUCCESS;

    unsigned char *EncodeBuffer;
    unsigned char *DecodeBuffer;

    FILE *fpWav = NULL;//wav file handle

    fpWav = fopen(input, "rb+");
    printf("读取语音信息:\n");
    if (fpWav)
    {
        fseek(fpWav,0,SEEK_END);
        dataLength = ftell (fpWav);

        DecodeBuffer = (unsigned char *)malloc(dataLength);
        memset(DecodeBuffer, 0, dataLength);
        fseek(fpWav,0,SEEK_SET);
        fread(DecodeBuffer, 1, dataLength, fpWav);
        fclose(fpWav);
        fpWav = NULL;
    }
    else
    {
        printf("Open wave file  failed!\n");
        result = RET_SRC_FILE_OPEN_FAILED;
    }

    // decode
    T_SBC_PARAMS encode_param;

    encode_param.samplingFrequency   = p_encode_param->samplingFrequency ;
    encode_param.blockNumber         = p_encode_param->blockNumber       ;
    encode_param.channelMode         = p_encode_param->channelMode       ;
    encode_param.allocMethod         = p_encode_param->allocMethod       ;
    encode_param.subbandNumber       = p_encode_param->subbandNumber     ;
    encode_param.bitpool             = p_encode_param->bitpool           ;

    sample_num = dataLength / VOICE_PCM_FRAME_SIZE * frame_size_after_enc;//30 * 128;
    EncodeBuffer = (unsigned char*)malloc(sample_num);
    memset(EncodeBuffer, 0, sample_num);

    /*sbc init*/
    sbc_init_encoder();
    int out_len = VOICE_PCM_FRAME_SIZE;

    for (int index = 0; index < dataLength/VOICE_PCM_FRAME_SIZE; index ++)
    {
        out_len = frame_size_after_enc;
        result = sbc_encode((unsigned char*)(DecodeBuffer + index*VOICE_PCM_FRAME_SIZE),
                            VOICE_PCM_FRAME_SIZE,
                            &encode_param,
                            (unsigned char *)(EncodeBuffer + index * frame_size_after_enc),
                            &out_len);
    }

    fpWav = fopen(output,"wb");
    if (fpWav)
    {
        fwrite(EncodeBuffer, 1, sample_num, fpWav);
        fclose(fpWav);
    }
    else
    {
        printf("open DecodedAudio error!");
        result = RET_DES_FILE_OPEN_FAILED;
    }

    if (EncodeBuffer != NULL)
        free(EncodeBuffer);
    else
        result = RET_DES_BUFFER_MOC_FAILED;
    if (DecodeBuffer != NULL)
        free(DecodeBuffer);
    else
        result = RET_SRC_BUFFER_MOC_FAILED;

    return result;
}



