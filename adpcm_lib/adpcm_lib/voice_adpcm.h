
#ifndef _VOICE_ADPCM_H_
#define _VOICE_ADPCM_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
*   common adpcm config 
*/
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
typedef int            int32_t;
typedef short          int16_t;

/*1. define adpcm codec type*/
#define     ADPCM_TYPE_IMA_16TO4           0
#define  	ADPCM_TYPE_OPT_16TO4           1
#define  	ADPCM_TYPE_OPT_16TO3_20BYTES   2/*20 byted coded left 1bits invalid per group */
#define     ADPCM_TYPE_OPT_16TO3_60BYTES   3/*60 bytes codec with all bits invalid per group*/

/*2. select adpcm codec type*/
#define ADPCM_CODEC_TYPE   ADPCM_TYPE_IMA_16TO4

#if (ADPCM_CODEC_TYPE == ADPCM_TYPE_IMA_16TO4)

//encode
#define VOICE_IMG_BITS                    0x04
#define IMG_OR                            0x00
#define IMG_AND                           0x0f
#define VOICE_FRAME_SIZE_AFTER_ENC        80

#define VOICE_SAMPLE_NUM  (VOICE_FRAME_SIZE_AFTER_ENC * 8 / VOICE_IMG_BITS)

//decode
#define VOICE_FRAME_SIZE                 80
#define SAMPLE_NUM_PER_BYTE             (8/4)

#elif (ADPCM_CODEC_TYPE == ADPCM_TYPE_OPT_16TO4)
//encode
#define VOICE_IMG_BITS                    0x04
#define IMG_OR                            0x00
#define IMG_AND                           0x0f
#define VOICE_FRAME_SIZE_AFTER_ENC        80

#define VOICE_SAMPLE_NUM  (VOICE_FRAME_SIZE_AFTER_ENC * 8 / VOICE_IMG_BITS)

//decode
#define VOICE_FRAME_SIZE                 80
#define SAMPLE_NUM_PER_BYTE              8/4

#elif (ADPCM_CODEC_TYPE == ADPCM_TYPE_OPT_16TO3_20BYTES)

//encode
#define VOICE_IMG_BITS                    0x03
#define IMG_OR                            0x01
#define IMG_AND                           0x0e
#define VOICE_FRAME_SIZE_AFTER_ENC        20

#define VOICE_SAMPLE_NUM  (VOICE_FRAME_SIZE_AFTER_ENC * 8 / VOICE_IMG_BITS)

//decode
#define VOICE_FRAME_SIZE                 20
#define SAMPLE_NUM_PER_BYTE              8/3

#elif (ADPCM_CODEC_TYPE == ADPCM_TYPE_OPT_16TO3_60BYTES)
//encode
#define VOICE_IMG_BITS                    0x03
#define IMG_OR                            0x01
#define IMG_AND                           0x0e
#define VOICE_FRAME_SIZE_AFTER_ENC        60

#define VOICE_SAMPLE_NUM  (VOICE_FRAME_SIZE_AFTER_ENC * 8 / VOICE_IMG_BITS)

//decode
#define VOICE_FRAME_SIZE                 60
#define SAMPLE_NUM_PER_BYTE              8/3

#endif/*endif ADPCM_CODEC_TYPE*/


/*================================================
 *    voice adpcm interface
 =================================================*/

int voice_adpcm_encoder(char* src_file, char* des_file);
int voice_adpcm_decoder(char* src_file, char* des_file);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif

