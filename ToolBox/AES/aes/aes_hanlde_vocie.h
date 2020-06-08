#ifndef _AES_HANDLE_VOICE_H_
#define _AES_HANDLE_VOICE_H_

#ifdef __cplusplus

extern "C" {

#endif

extern int aes_data_decrypt(char* pFileIn, char* pFileout);
extern int aes_data_encrypt(char* pFileIn, char* pFileout);

#ifdef __cplusplus
}
#endif

#endif
