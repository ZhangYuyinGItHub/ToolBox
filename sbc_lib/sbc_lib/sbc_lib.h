#ifndef SBC_LIB_H
#define SBC_LIB_H


class Sbc_lib
{

public:
    Sbc_lib();

    void sbc_encode(char* in, char* out);
    void sbc_decode(char* in, char* out);
};

#endif // SBC_LIB_H
