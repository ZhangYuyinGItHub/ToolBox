#ifndef _VOICE_SBC_H_
#define _VOICE_SBC_H_

#include "sbc.h"

typedef enum
{
	SBC_DECODE_SUCCESS = 0,
	SBC_DECODE_SRC_FILE_OPEN_ERROR,
	SBC_DECODE_DES_FILE_OPEN_ERROR,
	SBC_ENCODE_SRC_FILE_OPEN_ERROR,
	SBC_ENCODE_DES_FILE_OPEN_ERROR,
    SBC_ENCODE_SRC_FILE_LENGTH_ERROR,
    SBC_ENCODE_DES_FILE_LENGTH_ERROR,
}ret_Code;

int voice_sbc_decode(char *input, char *output);
int voice_sbc_encode(char *input, char *output, T_SBC_PARAMS *p_Encode_Param);

#endif
