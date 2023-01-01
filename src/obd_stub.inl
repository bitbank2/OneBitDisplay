//
// Stub functions to allow the obd_gfx functions to be used
// in a stand-alone mode without the display interface code
//
// obd_stub.inl
#include <string.h>
#define MAX_CACHE 192
static uint8_t u8Cache[MAX_CACHE];
static volatile uint8_t u8End = 0;
static uint8_t u8Temp[40]; // for stretched character drawing

const uint8_t ucMirror[256] PROGMEM =
     {0, 128, 64, 192, 32, 160, 96, 224, 16, 144, 80, 208, 48, 176, 112, 240,
      8, 136, 72, 200, 40, 168, 104, 232, 24, 152, 88, 216, 56, 184, 120, 248,
      4, 132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 212, 52, 180, 116, 244,
      12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220, 60, 188, 124, 252,
      2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 114, 242,
      10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250,
      6, 134, 70, 198, 38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246,
      14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254,
      1, 129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113, 241,
      9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249,
      5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245,
      13, 141, 77, 205, 45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253,
      3, 131, 67, 195, 35, 163, 99, 227, 19, 147, 83, 211, 51, 179, 115, 243,
      11, 139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219, 59, 187, 123, 251,
      7, 135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 215, 55, 183, 119, 247,
      15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255};

uint8_t pgm_read_byte(const uint8_t *p)
{
	return p[0];
} /* pgm_read_byte() */

// Write raw (unfiltered) bytes directly to I2C or SPI
static void RawWrite(OBDISP *pOBD, unsigned char *pData, int iLen)
{
    (void)pOBD; (void)pData; (void)iLen;
} /* RawWrite() */

void obdSetPosition(OBDISP *pOBD, int x, int y, int bRender)
{
int iPitch = pOBD->width;

   y >>= 3; // DEBUG - since we address the display by lines of 8 pixels
   pOBD->iScreenOffset = (y*iPitch)+x;

} /* obdSetPosition() */

void obdWriteDataBlock(OBDISP *pOBD, unsigned char *ucBuf, int iLen, int bRender)
{
int iPitch, iBufferSize;
int iRedOffset = 0;

  iPitch = pOBD->width;
  iBufferSize = iPitch * ((pOBD->height+7) / 8);
    if (pOBD->iFG == OBD_RED) {
        iRedOffset = pOBD->width * ((pOBD->height+7)/8);
    }
// Keep a copy in local buffer
if (pOBD->ucScreen && (iLen + pOBD->iScreenOffset) <= iBufferSize)
{
  memcpy(&pOBD->ucScreen[iRedOffset + pOBD->iScreenOffset], ucBuf, iLen);
  pOBD->iScreenOffset += iLen;
  // wrap around ?
  if (pOBD->iScreenOffset >= iBufferSize)
    pOBD->iScreenOffset -= iBufferSize;
}
} /* obdWriteDataBlock() */

