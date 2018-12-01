#include "sbc_lib.h"
#include "voice.h"


Sbc_lib::Sbc_lib()
{
}

void Sbc_lib::sbc_encode(char *in, char *out)
{
    voice_sbc(in, out);
}
