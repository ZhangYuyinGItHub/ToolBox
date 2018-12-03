#ifndef SBC_LIB_H
#define SBC_LIB_H


class Sbc_lib
{

public:
    Sbc_lib();

    int sbc_encode(char* in, char* out);
    int sbc_decode(char* in, char* out);
};

#endif // SBC_LIB_H
