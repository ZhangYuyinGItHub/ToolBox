#include <stdio.h>
#include <stdlib.h>
#include "image_header.h"
#include "aes.h"
#include "aes_hanlde_vocie.h"

char *program = "encrypt";

int sbc_frame_len = 83;
int sbc_header_len = 3;
#define SBC_SYNC_BYTE 0x9C


void usage(void)
{
    printf("%s <binary file> <key file> <aes mode - 0: ECB, 1: CBC>\n", program);
}

unsigned int read_file(char *p_filename, unsigned char file_buf[])
{
    FILE *fp;
    unsigned char ch;
    unsigned int length = 0;

    //Read file
    if((fp = fopen(p_filename,"rb")) == NULL)
    {
        fprintf(stderr, "Cannot open file %s!\n", p_filename);
        return 0;
    }

    ch = fgetc(fp);
    while(!feof(fp))
    {
        file_buf[length++] = ch;
        ch = fgetc(fp);
    }

    fclose(fp);
    return length;
}

unsigned int write_file(char *p_filename, unsigned char file_buf[], unsigned int len)
{
    FILE *fp;
    unsigned int length = 0;

    if((fp = fopen(p_filename,"wb")) == NULL)
    {
        fprintf(stderr, "Cannot open file %s!\n", p_filename);
        return 0;
    }

    if (length = fwrite(file_buf, 1, len, fp) != len)
    {
        printf("want to write %d bytes, write %d bytes!\n", len, length);
    }

    fclose(fp);

    return length;
}

static void reverse_16(unsigned char buf[], int size)
{
	int i;

	if (size % 16 != 0)
	{
		fprintf(stderr, "reverse_16 error, size is not a multiple of 16.\n");
		return;
	}

	for (i = 0; i < size / 16; ++i)
	{
		int j;
		char temp;
		unsigned char *pBuf = buf + 16 * i;
		for (j = 0; j < 8; ++j)
		{
			temp = pBuf[j];
			pBuf[j] = pBuf[15 - j];
			pBuf[15 - j] = temp;
		}
	}
}

int encrypt_work_code(uint8_t *binary, uint32_t binary_size, uint8_t *key, uint8_t *iv)
{
    int ret = 0;
    struct AES_ctx ctx;

    int encrypted_len = binary_size;
	int frame_cnt = binary_size / sbc_frame_len;
	int loop_cnt = (sbc_frame_len - sbc_header_len) / 16;
	int frame_index;
	int loop_index;

    /* hw aes limitation */
    reverse_16(key, 16);

    AES_init_ctx(&ctx, key);
    {
        for (frame_index = 0;frame_index<frame_cnt;frame_index += 1)
        {
			uint8_t *encrypted_buf = binary + (sbc_frame_len*frame_index) + sbc_header_len;
			for (loop_index =0; loop_index<loop_cnt;loop_index++)
			{
				reverse_16(encrypted_buf, 16);
				AES_ECB_encrypt(&ctx, encrypted_buf);
				reverse_16(encrypted_buf, 16);
				encrypted_buf += 16;
			}
            
        }
    }

    return ret;
}

int decrypt_work_code(uint8_t *binary, uint32_t binary_size, uint8_t *key, uint8_t *iv)
{
	int ret = 0;
	struct AES_ctx ctx;

	int encrypted_len = binary_size;
	int frame_cnt = binary_size / sbc_frame_len;
	int loop_cnt = (sbc_frame_len - sbc_header_len) / 16;
	int frame_index;
	int loop_index;

	/* hw aes limitation */
	reverse_16(key, 16);

	AES_init_ctx(&ctx, key);
	{
		for (frame_index = 0; frame_index<frame_cnt; frame_index += 1)
		{
			uint8_t *encrypted_buf = binary + (sbc_frame_len*frame_index) + sbc_header_len;
			for (loop_index = 0; loop_index<loop_cnt; loop_index++)
			{
				reverse_16(encrypted_buf, 16);
				AES_ECB_decrypt(&ctx, encrypted_buf);
				reverse_16(encrypted_buf, 16);
				encrypted_buf += 16;
			}

		}
	}

	return ret;
}
int aes_data_decrypt(char* pFileIn, char* pFileout)
{
    //char *binary;
    //char *binary_out;// [512] = { 0 };
    uint8_t *binary_buf;
    uint32_t binary_size;
    uint32_t aes_key_size;
    uint8_t iv[]  = {0xAB, 0xC4, 0x43, 0x1A, 0x07, 0x57, 0xD8, 0x57, 0x25, 0xC7, 0xEE, 0xDE, 0x8B, 0x9C, 0xE7, 0x8D};
	uint8_t aes_key_buf[16] = {0x4f, 0x8b, 0xad, 0xe8, 0x62, 0xe8, 0x6c, 0x2c, 0xa1, 0x3d, 0xdc, 0xd3, 0x1e, 0x45, 0x59, 0x91};

    //binary = "codec_test_sample_rhqc.bin";// argv[1];
    //binary_out = "codec_test_sample01.bin";

    if ((binary_buf = malloc(1024 * 1024)) == NULL)
    {
        fprintf(stderr, "malloc buffer for binary fail!\n");
        return 2;
    }

    binary_size = read_file(pFileIn, binary_buf);
    aes_key_size = 16;

	decrypt_work_code(binary_buf, binary_size, aes_key_buf, iv);

    //sprintf(binary_out, "%s.encrypted", binary);
    write_file(pFileout, binary_buf, binary_size);

    if (binary_buf)
        free(binary_buf);

    return 0;
}

int aes_data_encrypt(char* pFileIn, char* pFileout)
{
    uint8_t *binary_buf;
    uint32_t binary_size;
    uint32_t aes_key_size;
    uint8_t iv[] = {0xAB, 0xC4, 0x43, 0x1A, 0x07, 0x57, 0xD8, 0x57, 0x25, 0xC7, 0xEE, 0xDE, 0x8B, 0x9C, 0xE7, 0x8D};
    uint8_t aes_key_buf[16] = {0x4f, 0x8b, 0xad, 0xe8, 0x62, 0xe8, 0x6c, 0x2c, 0xa1, 0x3d, 0xdc, 0xd3, 0x1e, 0x45, 0x59, 0x91};

    if ((binary_buf = malloc(1024 * 1024)) == NULL)
    {
        fprintf(stderr, "malloc buffer for binary fail!\n");
        return 2;
    }

    binary_size = read_file(pFileIn, binary_buf);
    aes_key_size = 16;

    encrypt_work_code(binary_buf, binary_size, aes_key_buf, iv);

    write_file(pFileout, binary_buf, binary_size);

    if (binary_buf)
        free(binary_buf);

    return 0;
}
