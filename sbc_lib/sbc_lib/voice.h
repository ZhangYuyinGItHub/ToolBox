#ifndef VOICE_H
#define VOICE_H

#include "sbc.h"

typedef enum{
    RET_CODE_SUCCESS = 0,
    RET_SRC_FILE_NOT_EXIST,
    RET_DES_FILE_NOT_EXIST,
    RET_SRC_FILE_OPEN_FAILED,
    RET_DES_FILE_OPEN_FAILED,
    RET_SRC_BUFFER_MOC_FAILED,
    RET_DES_BUFFER_MOC_FAILED,
    RET_MAX_NUM,
}RET_CODE;

extern int voice_sbc_encoder(char *input, char *output, T_SBC_PARAMS *p_encode_param);
extern int voice_sbc_decoder(char *input, char *output);

#endif // VOICE_H
