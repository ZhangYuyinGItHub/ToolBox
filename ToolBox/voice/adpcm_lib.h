#ifndef ADPCM_LIB_H
#define ADPCM_LIB_H

class Adpcm_lib
{

public:
    Adpcm_lib();

    int adpcm_encoder(char* src_file, char* des_file);
    int adpcm_decoder(char* src_file, char* des_file);
};

#endif // ADPCM_LIB_H
