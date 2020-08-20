// voice.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include <stdlib.h> 
#include <string.h>

#include "sbc.h"
#include "voice.h"


//void main()
//{
//	T_SBC_PARAMS Encode_Param;
//	Encode_Param.samplingFrequency = SBC_FREQU16000;
//	Encode_Param.blockNumber = SBC_BLOCKS16;
//	Encode_Param.channelMode = SBC_MODE_JOINT;
//	Encode_Param.allocMethod = SBC_ALLOCLOUDNESS;
//	Encode_Param.subbandNumber = SBC_SUBBANDS8;
//	Encode_Param.bitpool = 53;

//	voice_sbc_encode("audio1.PCM", "audio2.dat", &Encode_Param);
//	voice_sbc_decode("audio2.dat", "audio3.pcm");
//}


int voice_sbc_decode(char *input, char *output)
{
    int ret = SBC_DECODE_SUCCESS;
    int dataLength;//length of encode buffer, half of sample number

    unsigned char *EncodeBuffer;
    short *DecodeBuffer;

    FILE *fpSrc = NULL;
    FILE *fpDes = NULL;

    /*0. open src file and des file*/
    fpSrc = fopen(input, "rb");
    printf("[Decode] SBC OPen Src File :<%s>\n", input);
    if (!fpSrc)
    {
        printf("[Decode] SBC Open Src File Failed!\n");
        return SBC_DECODE_SRC_FILE_OPEN_ERROR;
    }
    else
    {
        int dataLength = 0;
        fseek(fpSrc,0,SEEK_END);
        dataLength = ftell (fpSrc);
        if (dataLength == 0)
            return SBC_ENCODE_SRC_FILE_LENGTH_ERROR;
    }

    fpDes = fopen(output, "wb");
    printf("[Decode] SBC OPen Des File :<%s>\n", output);
    if (!fpDes)
    {
        printf("[Decode] SBC OPen Des File Failed!");
        free(fpSrc);
        return SBC_DECODE_DES_FILE_OPEN_ERROR;
    }

    /* 1. get sbc param*/
    int out_frame_len = 0;
    int in_frame_len = 0;
    unsigned char arr[3] = {0};
    fseek(fpSrc, 0, SEEK_SET);
    fread(arr, 1, 3, fpSrc);
    sbc_get_decode_ioparam(arr, 3, &out_frame_len, &in_frame_len);

    fseek(fpSrc, 0, SEEK_END);
    dataLength = ftell (fpSrc);

    /*2. apply encode frame buffer*/
    EncodeBuffer = (unsigned char*)malloc(in_frame_len);
    memset(EncodeBuffer, 0, in_frame_len);

    DecodeBuffer = (short*)malloc(out_frame_len);
    memset(DecodeBuffer, 0, out_frame_len);

    /*3. sbc decode init*/
    sbc_init_decoder();

    /*4. decode sbc*/
    int result = 0;
    for (int index = 0; index < dataLength / in_frame_len; index ++)
    {
        /*read in frame data*/
        fseek(fpSrc, in_frame_len * index, SEEK_SET);
        fread(EncodeBuffer, 1, in_frame_len, fpSrc);

        /*read out frame data*/
        fseek(fpDes, out_frame_len * index, SEEK_SET);
        fwrite(DecodeBuffer, 1, out_frame_len, fpDes);

        /*decode*/
        result = sbc_decode(EncodeBuffer,
                            in_frame_len,
                            (unsigned char *)(DecodeBuffer),
                            &out_frame_len);
    }

    /*5. release buffer*/
    if (fpSrc)
        fclose(fpSrc);
    if (fpDes)
        fclose(fpDes);
    if (NULL != EncodeBuffer)
        free(EncodeBuffer);
    if (NULL != DecodeBuffer)
        free(DecodeBuffer);

    return ret;
}

/**
*  @brief voice encode api
*/
int voice_sbc_encode(char *input, char *output, T_SBC_PARAMS *p_Encode_Param)
{
    int dataLength;//length of encode buffer, half of sample number
    int sample_num;//sample number

    unsigned char *EncodeBuffer;
    unsigned char *DecodeBuffer;

    FILE *fpWav = NULL;//wav file handle

    /*0. get src file*/
    fpWav = fopen(input, "rb");
    printf("[Encode] SBC OPen Src File: <%s>\n", input);
    if (!fpWav)
    {
        printf("[Encode] SBC OPen Src file Failed!\n");
        return SBC_ENCODE_SRC_FILE_OPEN_ERROR;
    }

    /*1. apply src file buffer*/
    fseek(fpWav,0,SEEK_END);
    dataLength = ftell (fpWav);
    DecodeBuffer = (unsigned char *)malloc(dataLength);
    memset(DecodeBuffer, 0, dataLength);
    fseek(fpWav,0,SEEK_SET);
    fread(DecodeBuffer, 1, dataLength, fpWav);
    fclose(fpWav);
    fpWav = NULL;

    /*2. get sbc encode param*/
    int in_len = 0;
    int out_len = 0;
    sbc_get_encode_ioparam(p_Encode_Param, &in_len, &out_len);

    /*3. apply encode buffer*/
    int result;
    sample_num = dataLength / in_len * out_len;
    EncodeBuffer = (unsigned char*)malloc(sample_num);
    memset(EncodeBuffer, 0, sample_num);

    /*4. sbc init and encode*/
    sbc_init_encoder();
    for (int index = 0; index < dataLength/in_len; index ++)
    {
        result = sbc_encode((unsigned char*)(DecodeBuffer + index*in_len),
                            in_len,
                            p_Encode_Param,
                            (unsigned char *)(EncodeBuffer + index * out_len),
                            &out_len);
    }

    /*5. open des file*/
    fpWav = fopen(output,"wb");
    printf("[Encode] SBC OPen Des File: <%s>\n", output);
    if (fpWav)
    {
        fwrite(EncodeBuffer, 1, sample_num, fpWav);
        fclose(fpWav);
    }
    else
    {
        printf("[Encode] SBC OPen Des File Failed!");
    }

    /*6. release buffer applied*/
    if (NULL != EncodeBuffer)
        free(EncodeBuffer);
    if (NULL != DecodeBuffer)
        free(DecodeBuffer);

    return 0;
}

