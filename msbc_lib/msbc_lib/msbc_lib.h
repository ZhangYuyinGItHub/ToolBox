#ifndef MSBC_LIB_H
#define MSBC_LIB_H


class Msbc_lib
{

public:
    Msbc_lib();

    void msbc_decoder(char* input, char* output);
    void msbc_encoder(char* input, char* output);
};

#endif // MSBC_LIB_H
