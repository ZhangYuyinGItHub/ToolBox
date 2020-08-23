// ADPCM1.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "adpcm.h"
#include "voice_adpcm.h"

adpcm_state dec_state;
adpcm_encode_state enc_state;


/**
 *@brief    adpcm file encode function
 *@param    char* src_file       input file name;
 *          char* des_file       output file name;
 *@return   int 0 success.
 */
int voice_adpcm_encoder(char* src_file, char* des_file)
{
    int dataLength;//length of encode buffer, half of sample number
    int sample_num;//sample number

    unsigned char *EncodeBuffer;
    unsigned char *DecodeBuffer;

    if ((src_file == NULL) || (strlen(src_file) == 0))
        return 1;

    if ((des_file == NULL) || (strlen(des_file) == 0))
        return 1;

    FILE *fpFile = NULL;//wav file handle

    fpFile = fopen(src_file, "rb");
    //printf("[Encode] OPen Source File: [%s]!\n", src_file);
    if (fpFile)
    {
        fseek(fpFile,0,SEEK_END);
        dataLength = ftell (fpFile);

        if (dataLength == 0)
        {
            //printf("[Decode] Source File Length 0\n");
            return 0;
        }

        DecodeBuffer = (unsigned char*)malloc(dataLength);
        memset(DecodeBuffer, 0, dataLength);
        fseek(fpFile,0,SEEK_SET);
        fread(DecodeBuffer, 1, dataLength, fpFile);
        fclose(fpFile);
        fpFile = NULL;
    }
    else
    {
        //printf("[Encode] OPen Source File Failed!\n");
        return 1;
    }

    sample_num = (dataLength / VOICE_SAMPLE_NUM /2)* VOICE_FRAME_SIZE_AFTER_ENC ;
    EncodeBuffer = (unsigned char*)malloc(sample_num);
    memset(EncodeBuffer, 0, sample_num);

#if (ADPCM_CODEC_TYPE != ADPCM_TYPE_IMA)
    for (int32_t i = 0; i < (dataLength / VOICE_SAMPLE_NUM / 2) ; i++)
    {
        enc_state.inp = (int16_t *)DecodeBuffer + VOICE_SAMPLE_NUM * i;
        enc_state.out = EncodeBuffer + VOICE_FRAME_SIZE_AFTER_ENC * i;
        enc_state.len = VOICE_SAMPLE_NUM;
        enc_state.imaSize = VOICE_IMG_BITS;
        enc_state.imaOr = IMG_OR;
        enc_state.imaAnd = IMG_AND;
        encode_adpcm(&enc_state);
    }
#elif (ADPCM_CODEC_TYPE == ADPCM_TYPE_IMA)
    for (int32_t i = 0; i < (dataLength / VOICE_SAMPLE_NUM / 2) ; i++)
    {
        adpcm_coder((short*)DecodeBuffer + VOICE_SAMPLE_NUM * i,
                    EncodeBuffer+VOICE_FRAME_SIZE_AFTER_ENC * i,
                    VOICE_SAMPLE_NUM,
                    &dec_state);
    }
#endif

    fpFile = fopen(des_file,"wb");
    //printf("[Encode] OPen Destination File: [%s]!\n\n", des_file);
    if (fpFile)
    {
        fwrite(EncodeBuffer, 1, sample_num, fpFile);
        fclose(fpFile);
    }
    else
    {
        //printf("[Encode] OPen Destination File Failed!\n");
    }

    return 0;
}

/**
 *@brief    adpcm file decode function
 *@param    char* src_file       input file name;
 *          char* des_file       output file name;
 *@return   int 0 success.
 */
int voice_adpcm_decoder(char* src_file, char* des_file)
{
    int dataLength;//length of encode buffer, half of sample number
    int sample_num;//sample number

    unsigned char *EncodeBuffer;
    short *DecodeBuffer;

    if ((src_file == NULL) || (strlen(src_file) == 0))
        return 1;

    if ((des_file == NULL) || (strlen(des_file) == 0))
        return 1;

    FILE *fpFile = NULL;//wav file handle

    fpFile = fopen(src_file, "rb");
    //printf("[Decode] Source File: [%s]\n", src_file);

    if (fpFile)
    {
        fseek(fpFile,0,SEEK_END);
        dataLength = ftell (fpFile);

        if (dataLength == 0)
        {
            //printf("[Decode] Source File Length 0\n");
            return 0;
        }

        sample_num = dataLength * SAMPLE_NUM_PER_BYTE;

        EncodeBuffer = (unsigned char*)malloc(dataLength);
        memset(EncodeBuffer, 0, dataLength);
        fseek(fpFile,0,SEEK_SET);
        fread(EncodeBuffer, 1, dataLength, fpFile);
        fclose(fpFile);
        fpFile = NULL;
    }
    else
    {
        //printf("[Decode] OPen Source File Failed!\n");
        return 2;
    }

    DecodeBuffer = (short*)malloc(sizeof(short) * sample_num);
    memset(DecodeBuffer, 0, sizeof(short) * sample_num);

    //decode init
    dec_state.valprev = 0;
    dec_state.index = 0;
    dec_state.peak_level = -32768;
    dec_state.write_out_index = 0;

#if (ADPCM_CODEC_TYPE == ADPCM_TYPE_IMA)
    adpcm_decoder(EncodeBuffer, DecodeBuffer, dataLength, &dec_state);
#elif (ADPCM_CODEC_TYPE == ADPCM_TYPE_OPT_16TO4)
    decode_adpcm_16to4(EncodeBuffer, dataLength, DecodeBuffer, &dec_state);
#elif (ADPCM_CODEC_TYPE == ADPCM_TYPE_OPT_16TO3_60BYTES)

    decode_adpcm_16to3(EncodeBuffer, dataLength, DecodeBuffer, &dec_state);
#elif (ADPCM_CODEC_TYPE == ADPCM_TYPE_OPT_16TO3_20BYTES)
    {
        int n = 0;
        for (n=0; n<(dataLength/VOICE_FRAME_SIZE); n++)
        {
            decode_adpcm_16to3(EncodeBuffer+n*VOICE_FRAME_SIZE, VOICE_FRAME_SIZE, DecodeBuffer, &dec_state);
        }
        decode_adpcm_16to3(EncodeBuffer+n*(VOICE_FRAME_SIZE), dataLength%VOICE_FRAME_SIZE, DecodeBuffer, &dec_state);
    }
#endif

    fpFile = fopen(des_file,"wb");
    //printf("[Decode] OPen Destination File: [%s]!\n\n", des_file);
    if (fpFile)
    {
        fwrite(DecodeBuffer,sizeof(short), sample_num, fpFile);
        fclose(fpFile);
    }
    else
    {
        //printf("[Decode] OPen Destination File Failed!\n");
    }

    return 0;
}
