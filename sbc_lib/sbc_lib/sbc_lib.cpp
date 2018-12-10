#include "sbc_lib.h"
#include "voice.h"
#include "sbc.h"

Sbc_lib::Sbc_lib()
{
}

int Sbc_lib::sbc_encode(char *in, char *out, void* pParam)
{
    return voice_sbc_encoder(in, out, (T_SBC_PARAMS *)pParam);
}

int Sbc_lib::sbc_decode(char *in, char *out, void* pParam)
{
    return voice_sbc_decoder(in, out, (T_SBC_PARAMS *)pParam);
}
