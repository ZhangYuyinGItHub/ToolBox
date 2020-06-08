#ifndef _IMAGE_HEADER_H_
#define _IMAGE_HEADER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _RSA_PUBLIC_KEY
{
    uint8_t N[256];
    uint8_t E[4];
} RSA_PUBLIC_KEY;

typedef struct _IMG_CTRL_HEADER_FORMAT
{
    uint8_t ic_type;
    uint8_t secure_version;
    union
    {
        uint16_t value;
        struct
        {
            uint16_t xip: 1; // payload is executed on flash
            uint16_t enc: 1; // all the payload is encrypted
            uint16_t load_when_boot: 1; // load image when boot
            uint16_t enc_load: 1; // encrypt load part or not
            uint16_t enc_key_select: 3; // referenced to ENC_KEY_SELECT
            uint16_t not_ready : 1; //for copy image in ota
            uint16_t not_obsolete : 1; //for copy image in ota
            uint16_t integrity_check_en_in_boot : 1; // enable image integrity check in boot flow
            uint16_t rsvd: 6;
        };
    } ctrl_flag;
    uint16_t image_id;
    uint16_t crc16;
    uint32_t payload_len;
} T_IMG_CTRL_HEADER_FORMAT;

typedef struct
{
    union
    {
        uint32_t version;
        struct
        {
            uint32_t _version_major: 4;     //!< major version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_revision: 15; //!< revision version
            uint32_t _version_reserve: 5;   //!< reserved
        } sub_version;
    };
    uint32_t _version_commitid;     //!< git commit id
    uint8_t _customer_name[8];      //!< branch name for customer patch
} T_VERSION_FORMAT;

typedef struct _IMG_HEADER_FORMAT
{
    T_IMG_CTRL_HEADER_FORMAT ctrl_header;
    uint8_t uuid[16];
    uint32_t exe_base;
    uint32_t load_base;
    uint32_t load_len;
    uint32_t image_base;
    uint8_t rsvd0[4];
    uint32_t magic_pattern;
    uint8_t dec_key[16];
    uint8_t rsvd1[28];
    T_VERSION_FORMAT git_ver;
    RSA_PUBLIC_KEY rsaPubKey;
    uint8_t sha256[32];
    uint8_t rsvd2[68];
    uint32_t app_cb_signature;
    uint32_t app_cb_table_base_address;
} T_IMG_HEADER_FORMAT;

#ifdef __cplusplus
}
#endif

#endif /* _IMAGE_HEADER_H_ */
