//
//  main.c
//  OneBitDisplay library test program
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>

#include "../../src/OneBitDisplay.h"
#include "../../src/obd.inl"
#include "../../examples/prop_font_demo/FreeSerif12pt7b.h"
#include "notes.h"

void WriteBMP(const char *name, uint8_t *pData, int width, int height, int bpp)
{
    int bsize, lsize, i, iHeaderSize, iBodySize;
    uint8_t pBuf[128]; // holds BMP header
    FILE *outfile;
    
    bsize = width >> 3;
    lsize = (bsize + 3) & 0xfffc; // DWORD aligned
    iHeaderSize = 54;
    iHeaderSize += (1<<(bpp+2));
    iBodySize = lsize * height;
    i = iBodySize + iHeaderSize; // datasize
    memset(pBuf, 0, 54);
    pBuf[0] = 'B';
    pBuf[1] = 'M';
    pBuf[2] = i & 0xff;     // 4 bytes of file size
    pBuf[3] = (i >> 8) & 0xff;
    pBuf[4] = (i >> 16) & 0xff;
    pBuf[5] = (i >> 24) & 0xff;
    /* Offset to data bits */
    pBuf[10] = iHeaderSize & 0xff;
    pBuf[11] = (unsigned char)(iHeaderSize >> 8);
    pBuf[14] = 0x28;
    pBuf[18] = width & 0xff; // xsize low
    pBuf[19] = (unsigned char)(width >> 8); // xsize high
    i = -height; // top down bitmap
    pBuf[22] = i & 0xff; // ysize low
    pBuf[23] = (unsigned char)(i >> 8); // ysize high
    pBuf[24] = 0xff;
    pBuf[25] = 0xff;
    pBuf[26] = 1; // number of planes
    pBuf[28] = (uint8_t)bpp;
    pBuf[30] = 0; // uncompressed
    i = iBodySize;
    pBuf[34] = i & 0xff;  // data size
    pBuf[35] = (i >> 8) & 0xff;
    pBuf[36] = (i >> 16) & 0xff;
    pBuf[37] = (i >> 24) & 0xff;
    pBuf[54] = pBuf[55] = pBuf[56] = pBuf[57] = pBuf[61] = 0; // palette
    pBuf[58] = pBuf[59] = pBuf[60] = 0xff;
    outfile = fopen((char *)name, "w+b");
    if (outfile)
    {
        uint8_t *s = pData;
        fwrite(pBuf, 1, iHeaderSize, outfile);
        for (i=0; i<height; i++)
        {
            fwrite(s, 1, lsize, outfile);
            s += lsize;
        }
        fclose(outfile);
    }
} /* WriteBMP() */

int main(int argc, const char * argv[])
{
int width, height, out_pitch;
uint8_t *pOut, *pBitonal;
OBDISP obd;
    
    if (argc != 2) {
        printf("Usage: obd_test <outfile\n");
        return 0;
    }
    width = 640;
    height = 480;

    out_pitch = (width + 7) >> 3;
    pBitonal = malloc(out_pitch * height);
    pOut = malloc(out_pitch * height);

    obdCreateVirtualDisplay(&obd, width, height, pBitonal);
    // Create some simple content
    obdFill(&obd, 0xff, 1); // colors are inverted
    obdWriteString(&obd,0,0,0,"OneBitDisplay test program", FONT_12x16, 1, 1);
    obdWriteString(&obd,0,0,2,"Written to vertify fonts and GFX", FONT_16x16, 1, 1);
    obdWriteString(&obd,0,0,4,"This is the 8x8 fixed font", FONT_8x8, 1, 1);
    obdWriteString(&obd,0,0,5,"This is the smallest (6x8) font", FONT_6x8, 1, 1);
    obdWriteString(&obd,0,0,6,"This is the largest built-in font", FONT_16x32, 1, 1);
    obdWriteStringCustom(&obd, (GFXfont *)&FreeSerif12pt7b, 0, 100, (char *)"Custom (Adafruit_GFX format) fonts are supported too",0);
    for (int i=0; i<640; i+=8) {
       obdDrawLine(&obd, i, 128, 640-i, 479, 0, 0);
    }
    obdEllipse(&obd, 320, 240+64, 150,100, 0, 1);
    obdRectangle(&obd, 300, 240+32, 340, 240+96, 1, 1);
    obdLoadBMP(&obd, (uint8_t *)notes, 15, 240, 0);
    obdCopy(&obd, OBD_ROTATE_90 | OBD_MSB_FIRST | OBD_HORZ_BYTES | OBD_FLIP_VERT | OBD_FLIP_HORZ, pOut);
    WriteBMP(argv[1], pOut, height, width, 1);
    free(pOut);
    free(pBitonal);
    return 0;
} /* main() */
