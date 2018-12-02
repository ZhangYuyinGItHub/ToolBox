#include "sbc_lib.h"
#include "voice.h"


Sbc_lib::Sbc_lib()
{
}

void Sbc_lib::sbc_encode(char *in, char *out)
{
    voice_sbc_encoder(in, out);
}

void Sbc_lib::sbc_decode(char *in, char *out)
{
    voice_sbc_decoder(in, out);
}
