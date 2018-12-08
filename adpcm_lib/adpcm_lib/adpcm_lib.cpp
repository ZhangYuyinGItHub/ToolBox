#include "adpcm_lib.h"
#include "voice_adpcm.h"

Adpcm_lib::Adpcm_lib()
{
}

int Adpcm_lib::adpcm_encoder(char* src_file, char* des_file)
{
    return voice_adpcm_encoder(src_file, des_file);
}

int Adpcm_lib::adpcm_decoder(char* src_file, char* des_file)
{
    return voice_adpcm_decoder(src_file, des_file);
}
