#include "stdio.h"
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;
#include "msbc.h"
#include "MsbcVoice.h"


#define PCM_FRAME_SIZE							240
#define MSBC_FRAME_SIZE							57
#define MSBC_FRAME_SIZE_WITH_H2HEADER_PADDING	60

#define MSBC_BITPOOL	26
#define MSBC_BLOCKS		15
#define MSBC_CHANNELS	1
#define MSBC_JOINT		0
#define MSBC_SUBBANDS	8
#define MSBC_SAMPLERATE	16000

void voice_msbc_encoder(char* input, char* output)
{
    if ((input == NULL) || (strlen(input) == 0))
        return;

    if ((output == NULL) || (strlen(output) == 0))
        return;

	sbc_t msbc;
	msbc.bitpool = MSBC_BITPOOL;
	msbc.blocks = MSBC_BLOCKS;
	msbc.channels = MSBC_CHANNELS;
	msbc.joint = MSBC_JOINT;
	msbc.subbands = MSBC_SUBBANDS;
	msbc.rate = MSBC_SAMPLERATE;

	sbc_init(&msbc, 0);

    string rfile = input;
	ifstream rf;
	rf.open(rfile.c_str(),ios::binary | ios::in);
	streampos pos;
	rf.seekg(0, ios::end);
	pos = rf.tellg();
	rf.seekg(0, ios::beg);
	size_t FileLength = size_t(pos);

	char *ch = (char *)malloc(FileLength);
	memset(ch, 1, FileLength);

	rf.read(ch, FileLength);
	rf.close();

	size_t FrameCount = FileLength / 240;
    void *PcmBuffer = (void *)malloc(PCM_FRAME_SIZE);
	void *MsbcBuffer = (void *)malloc(60);

	ofstream wf;
    wf.open(output, ios::binary | ios::out);


	for (int i = 0; i < int(FrameCount); i++)
	{
		int encoded = 0;
		memset(PcmBuffer, 0, PCM_FRAME_SIZE);
		//memset(MsbcBuffer, 0, MSBC_FRAME_SIZE);
		memset(MsbcBuffer, 0, 60);

		memcpy(PcmBuffer, &ch[i*PCM_FRAME_SIZE], PCM_FRAME_SIZE);
		sbc_encode(&msbc, PcmBuffer, PCM_FRAME_SIZE, ((unsigned char*)MsbcBuffer+2),
			   MSBC_FRAME_SIZE, &encoded);

		if (encoded != MSBC_FRAME_SIZE)
			break;
		wf.write((char *)MsbcBuffer, MSBC_FRAME_SIZE+3);
	}
	wf.close();

	free(PcmBuffer);
	free(MsbcBuffer);
	free(ch);
	return ;
}

void voice_msbc_decoder(char* input, char* output)
{
    if ((input == NULL) || (strlen(input) == 0))
        return;

    if ((output == NULL) || (strlen(output) == 0))
        return;

	sbc_t msbc;
	msbc.bitpool = MSBC_BITPOOL;
	msbc.blocks = MSBC_BLOCKS;
	msbc.channels = MSBC_CHANNELS;
	msbc.joint = MSBC_JOINT;
	msbc.subbands = MSBC_SUBBANDS;
	msbc.rate = MSBC_SAMPLERATE;

	sbc_init(&msbc, 0);

    string rfile = input;
	ifstream rf;
	rf.open(rfile.c_str(),ios::binary | ios::in);
	streampos pos;
	rf.seekg(0, ios::end);
	pos = rf.tellg();
	rf.seekg(0, ios::beg);
	size_t FileLength = size_t(pos);

	char *ch = (char *)malloc(FileLength);
	memset(ch, 1, FileLength);

	rf.read(ch, FileLength);
	rf.close();

	//size_t FrameCount = FileLength / MSBC_FRAME_SIZE;
	size_t FrameCount = FileLength / 60;
	void *PcmBuffer = (void *)malloc(PCM_FRAME_SIZE);
	//void *MsbcBuffer = (void *)malloc(MSBC_FRAME_SIZE);
	void *MsbcBuffer = (void *)malloc(60);

	ofstream wf;
    wf.open(output, ios::binary | ios::out);


	for (int i = 0; i < int(FrameCount); i++)
	{
		int encoded = 0;
		memset(PcmBuffer, 0, PCM_FRAME_SIZE);
		//memset(MsbcBuffer, 0, MSBC_FRAME_SIZE);
		memset(MsbcBuffer, 0, 60);

		//memcpy(MsbcBuffer, &ch[i*MSBC_FRAME_SIZE], MSBC_FRAME_SIZE);
		memcpy(MsbcBuffer, &ch[i*60+2], MSBC_FRAME_SIZE);
		sbc_decode(&msbc, MsbcBuffer, MSBC_FRAME_SIZE, PcmBuffer, PCM_FRAME_SIZE, &encoded);//MsbcCodec
		if (encoded != PCM_FRAME_SIZE)
			break;
		wf.write((char *)PcmBuffer, PCM_FRAME_SIZE);
	}
	wf.close();

	free(PcmBuffer);
	free(MsbcBuffer);
	free(ch);
	return ;
}
