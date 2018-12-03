#include "sbc_lib.h"
#include "voice.h"


Sbc_lib::Sbc_lib()
{
}

int Sbc_lib::sbc_encode(char *in, char *out)
{
    return voice_sbc_encoder(in, out);
}

int Sbc_lib::sbc_decode(char *in, char *out)
{
    return voice_sbc_decoder(in, out);
}
