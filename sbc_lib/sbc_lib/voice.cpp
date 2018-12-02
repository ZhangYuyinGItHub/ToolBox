// voice.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include <stdlib.h> 
#include <string.h>
//#include "adpcm.h"
#include "wav.h"
#include "sbc.h"


unsigned char WaveHeader[] = {0x52,0x49,0x46,0x46,0x24,0xa5,0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
	0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x80,0x3e,0x00,0x00,0x00,0x7d,0x00,0x00,
	0x02,0x00,0x10,0x00,0x64,0x61,0x74,0x61,0x00,0xa5,0x00,0x00,0x0d,0x0a};
 
/*
 * @brief voice sbc decoder.
 */
int voice_sbc_decoder(char *input, char *output)
{
    if ((input != NULL) || (input == ""))
        return -1;

    if ((output != NULL) || (output == ""))
        return -1;

	wav_pcm_header44* hwav = (wav_pcm_header44*)WaveHeader;;//wav file header 

	int dataLength;//length of encode buffer, half of sample number
	int sample_num;//sample number

	unsigned char *EncodeBuffer;
	short *DecodeBuffer;

	FILE *fpWav = NULL;//wav file handle 
	//fpWav = fopen("audio_data.dat", "rb"); 

	//char *filename = "../data/AudioRecord";

	/*
	char filenameSrt[30] = {0};
	strcat(filenameSrt,filename);
	strcat(filenameSrt,".dat");

	char filenameDes[30] = {0};
	strcat(filenameDes,filename);
	strcat(filenameDes,".wav");
	*/
	fpWav = fopen(input, "rb");
	printf("读取语音信息:\n");
	if (fpWav)   
	{
		fseek(fpWav,0,SEEK_END);
		dataLength = ftell (fpWav); 
		sample_num = dataLength / 36*128;//30 * 128;

		//update wav header info
		hwav->SubChun2Size = sample_num * 2;//16bit
		hwav->ChunkSize = hwav->SubChun2Size + 36;//这个值在16k时是36
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
	}

	// decode
	T_SBC_PARAMS encode_param;
	int result;

	encode_param.samplingFrequency = SBC_FREQU16000;
	encode_param.blockNumber = SBC_BLOCKS16;
	encode_param.channelMode = SBC_MODE_MONO;
	encode_param.allocMethod = SBC_ALLOCLOUDNESS;
	encode_param.subbandNumber = SBC_SUBBANDS8;
	encode_param.bitpool = 14;

	DecodeBuffer = (short*)malloc(sizeof(short) * sample_num);
	memset(DecodeBuffer, 0, sizeof(short) * sample_num);

	/*sbc init*/
	sbc_init_decoder();
	int out_len = 256;

	for (int index = 0; index < dataLength/36; index ++)
	{
		out_len = 256;
	    result = sbc_decode(EncodeBuffer + index*36, 
			                36, 
							(unsigned char *)(DecodeBuffer + index * 128), 
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
	}
	

    free(EncodeBuffer);
    free(DecodeBuffer);

	return 0;
}

/**
*  @brief voice sbc encode
*/
int voice_sbc_encoder(char *input, char *output)
{
    if ((input != NULL) || (input == ""))
        return -1;

    if ((output != NULL) || (output == ""))
        return -1;

    int dataLength;//length of encode buffer, half of sample number
    int sample_num;//sample number

    unsigned char *EncodeBuffer;
    unsigned char *DecodeBuffer;

    FILE *fpWav = NULL;//wav file handle

    fpWav = fopen(input, "rb");
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
    }

    // decode
    T_SBC_PARAMS encode_param;
    int result;

    encode_param.samplingFrequency = SBC_FREQU16000;
    encode_param.blockNumber = SBC_BLOCKS16;
    encode_param.channelMode = SBC_MODE_MONO;
    encode_param.allocMethod = SBC_ALLOCLOUDNESS;
    encode_param.subbandNumber = SBC_SUBBANDS8;
    encode_param.bitpool = 14;

    sample_num = dataLength / 256 * 36;//30 * 128;
    EncodeBuffer = (unsigned char*)malloc(sample_num);
    memset(DecodeBuffer, 0, sample_num);

    /*sbc init*/
    sbc_init_encoder();
    int out_len = 256;

    for (int index = 0; index < dataLength/256; index ++)
    {
        out_len = 36;
        result = sbc_encode((unsigned char*)(DecodeBuffer + index*256),
                            256,
                            &encode_param,
                            (unsigned char *)(EncodeBuffer + index * 36),
                            &out_len);
        //printf("index = %d, out_len = %d\n", index, out_len);
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
    }

    free(EncodeBuffer);
    free(DecodeBuffer);

    return 0;
}



