#include "msbc_lib.h"
#include "MsbcVoice.h"


Msbc_lib::Msbc_lib()
{
}

void Msbc_lib::msbc_encoder(char* input, char* output)
{
    voice_msbc_encoder(input, output);
}

void Msbc_lib::msbc_decoder(char* input, char* output)
{
    voice_msbc_decoder(input, output);
}
