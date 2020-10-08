#ifndef _DISPLAY_BITMATP_UTILS_H_
#define _DISPLAY_BITMATP_UTILS_H_

class DisplayBitmapUtils {
    public:
        static inline void DisplayBitmapCopy(uint8_t *dst, const uint8_t *src, int width){
            for (int i=0; i<width; i++)
                dst[i] = src[i] ;
        }

        static inline void DisplayBitmapAdd(uint8_t *dst, const uint8_t *src1, const uint8_t *src2, int width){
            for (int i=0; i<width; i++)
                dst[i] = src1[i] | src2[i];
        }
};

#endif