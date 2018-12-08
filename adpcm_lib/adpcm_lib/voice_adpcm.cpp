// ADPCM1.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include <stdlib.h> 
#include <string.h>
#include "adpcm.h"
#include "wav.h"
#include "voice_adpcm.h"


adpcm_state dec_state = {0};
adpcm_encode_state enc_state = {0};

unsigned char WavHeader[] = {0x52,0x49,0x46,0x46,0x24,0xa5,0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6d,0x74,0x20,
	0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x80,0x3e,0x00,0x00,0x00,0x7d,0x00,0x00,
	0x02,0x00,0x10,0x00,0x64,0x61,0x74,0x61,0x00,0xa5,0x00,0x00,0x0d,0x0a};

//void main()
//{
//	voice_adpcm_encoder("test_radio.pcm", "test_radio.dat");
//	voice_adpcm_decoder("test_radio.dat", "test_radio_ima.wav");

//	getchar();
//}

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

	FILE *fpWav = NULL;//wav file handle
	
	fpWav = fopen(src_file, "rb");
	printf("[Encode] OPen Source File: [%s]!\n", src_file); 
	if (fpWav)   
	{   
		fseek(fpWav,0,SEEK_END);
		dataLength = ftell (fpWav); 

		DecodeBuffer = (unsigned char*)malloc(dataLength); 
		memset(DecodeBuffer, 0, dataLength);
		fseek(fpWav,0,SEEK_SET);
		fread(DecodeBuffer, 1, dataLength, fpWav);
		fclose(fpWav);   
		fpWav = NULL;  
	}   
	else   
	{   
		printf("[Encode] OPen Source File Failed!\n"); 
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

	fpWav = fopen(des_file,"wb");
	printf("[Encode] OPen Destination File: [%s]!\n\n", des_file); 
	if (fpWav)
	{		
		fwrite(EncodeBuffer, 1, sample_num, fpWav);
		fclose(fpWav);
	}
	else
	{
		printf("[Encode] OPen Destination File Failed!\n");
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
    wav_pcm_header44* hwav = (wav_pcm_header44*)WavHeader;;//wav file header

	int dataLength;//length of encode buffer, half of sample number
	int sample_num;//sample number

	unsigned char *EncodeBuffer;
	short *DecodeBuffer;

	if ((src_file == NULL) || (strlen(src_file) == 0))
		return 1;

	if ((des_file == NULL) || (strlen(des_file) == 0))
		return 1;

	FILE *fpWav = NULL;//wav file handle 
	
	fpWav = fopen(src_file, "rb");
	printf("[Decode] Source File: [%s]\n", src_file);

	if (fpWav)   
	{   
		fseek(fpWav,0,SEEK_END);
		dataLength = ftell (fpWav); 

		sample_num = dataLength * SAMPLE_NUM_PER_BYTE;

		//update wav header info
		hwav->SampleRate = 16000;//decode后降采样为8k
		hwav->SubChun2Size = sample_num * 2;//16bit
		hwav->ChunkSize = hwav->SubChun2Size + 18;//这个值在16k时是36
		hwav->ByteRate = 16000*2;//8k * 2

		EncodeBuffer = (unsigned char*)malloc(dataLength); 
		memset(EncodeBuffer, 0, dataLength);
		fseek(fpWav,0,SEEK_SET);
		fread(EncodeBuffer, 1, dataLength, fpWav);
		fclose(fpWav);   
		fpWav = NULL;  
	}   
	else   
	{   
		printf("[Decode] OPen Source File Failed!\n");
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

	fpWav = fopen(des_file,"wb");
	printf("[Decode] OPen Destination File: [%s]!\n\n", des_file); 
	if (fpWav)
	{
		fwrite(hwav,sizeof(wav_pcm_header44), 1,fpWav);
		fwrite(DecodeBuffer,sizeof(short), sample_num, fpWav);
		fclose(fpWav);
	}
	else
	{
		printf("[Decode] OPen Destination File Failed!\n");
	} 

	return 0;
}
