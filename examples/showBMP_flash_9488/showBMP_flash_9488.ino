#if defined(ESP8266)
#define FLASH_CS D3
#else
#define FLASH_CD 3        //select 3 or 5 on ILI9488 Protoshield
#endif

#include <SPI.h>          // f.k. for Arduino-1.5.2
//#include <MCUFRIEND_kbv.h>
//MCUFRIEND_kbv tft;
//#include <ILI9341_kbv.h>
//ILI9341_kbv tft;
#include <ILI9488_kbv.h>
ILI9488_kbv tft;
//#include <TFT_eSPI.h>       // no readID(), different setAddrWindow()
//TFT_eSPI tft;

#if defined(_TFT_eSPIH_)
#define setAddrWindow setWindow
#endif

#include "SPI_Flash.h"

SPISettings flashSetting(24000000, MSBFIRST, SPI_MODE0);
SPISettings tftSetting(24000000, MSBFIRST, SPI_MODE0);

//extern void read_spi_mem(uint32_t loc, uint8_t *buf, int16_t n, int select);

void read_spi_mem(uint32_t loc, uint8_t *buf, int16_t n, int select)
{
    SPI.beginTransaction(flashSetting);
    digitalWrite(select, LOW);
    SPI.transfer(0x03);
    SPI.transfer(loc >> 16);
    SPI.transfer(loc >> 8);
    SPI.transfer(loc >> 0);
#if defined(ESP8266)
    SPI.transferBytes(NULL, buf, n);
#else
    SPI.transfer(buf, n);   //stupid Arduino method
/*   
    while (n-- > 0) {
        *buf++ = SPI.transfer(0x00);
    }
*/
#endif 
    digitalWrite(select, HIGH);
    SPI.endTransaction();
    SPI.beginTransaction(tftSetting);
}

void hexdump(uint32_t loc, uint8_t *block, int16_t n)
{
    int16_t cnt;
    char buf[20], ascbuf[17], *p, wid = 16;
    while (n > 0) {
        sprintf(buf, "%06lX:", loc);
        Serial.print(buf);
        p = ascbuf;
        cnt = n;
        if (cnt > wid) cnt = wid;
        loc += cnt;
        n -= cnt;
        while (cnt--) {
            uint8_t c = *block++;
            *p++ = (c >= ' ' && c < 128) ? c : '.';
            sprintf(buf, " %02X", c);
            Serial.print(buf);
        }
        *p = 0;
        sprintf(buf, " *%s*", ascbuf);
        Serial.println(buf);
    }
}

#define BMPIMAGEOFFSET 54

#define PALETTEDEPTH   8
#define BUFFPIXEL 20

uint16_t flashread16(SPI_Flash& f) {
    uint16_t result;         // read little-endian
    f.read(&result, sizeof(result));
    return result;
}

uint32_t flashread32(SPI_Flash& f) {
    uint32_t result ;
    f.read(&result, sizeof(result));
    return result;
}

uint8_t showBMP_flash(int32_t ads, int x, int y)
{
    SPI_Flash bmpFile;
    int bmpWidth, bmpHeight;    // W+H in pixels
    uint8_t bmpDepth;           // Bit depth (currently must be 24, 16, 8, 4, 1)
    uint32_t bmpImageoffset;    // Start of image data in file
    uint32_t rowSize;           // Not always = bmpWidth; may have padding
    uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
    uint16_t lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], *palette = NULL;
    uint8_t bitmask, bitshift;
    boolean flip = true;        // BMP is stored bottom-to-top
    int w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;
    uint32_t pos;               // seek position
    boolean is565 = false;      //

    uint16_t bmpID;
    uint16_t n;                 // blocks read
    uint8_t ret;

    if ((x >= tft.width()) || (y >= tft.height()))
        return 1;               // off screen

    //    bmpFile = SD.open(nm);      // Parse BMP header
    bmpFile.open(ads);
    bmpID = flashread16(bmpFile);    // BMP signature
    (void) flashread32(bmpFile);     // Read & ignore file size
    (void) flashread32(bmpFile);     // Read & ignore creator bytes
    bmpImageoffset = flashread32(bmpFile);       // Start of image data
    (void) flashread32(bmpFile);     // Read & ignore DIB header size
    bmpWidth = flashread32(bmpFile);
    bmpHeight = flashread32(bmpFile);
    n = flashread16(bmpFile);        // # planes -- must be '1'
    bmpDepth = flashread16(bmpFile); // bits per pixel
    pos = flashread32(bmpFile);      // format
    if (bmpID != 0x4D42) ret = 2; // bad ID
    else if (n != 1) ret = 3;   // too many planes
    else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
    else {
        bool first = true;
        is565 = (pos == 3);               // ?already in 16-bit format
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
        if (bmpHeight < 0) {              // If negative, image is in top-down order.
            bmpHeight = -bmpHeight;
            flip = false;
        }

        w = bmpWidth;
        h = bmpHeight;
        if ((x + w) >= tft.width())       // Crop area to be loaded
            w = tft.width() - x;
        if ((y + h) >= tft.height())      //
            h = tft.height() - y;

        if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
            bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
            bitmask = 0xFF;
            if (bmpDepth < 8)
                bitmask >>= bmpDepth;
            bitshift = 8 - bmpDepth;
            n = 1 << bmpDepth;
            lcdbufsiz -= n;
            palette = lcdbuffer + lcdbufsiz;
            for (col = 0; col < n; col++) {
                pos = flashread32(bmpFile);    //map palette to 5-6-5
                palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
            }
        }

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
        for (row = 0; row < h; row++) { // For each scanline...
            // Seek to start of scan line.  It might seem labor-
            // intensive to be doing this on every line, but this
            // method covers a lot of gritty details like cropping
            // and scanline padding.  Also, the seek only takes
            // place if the file position actually needs to change
            // (avoids a lot of cluster math in SD library).
            uint8_t r, g, b, *sdptr;
            int lcdidx, lcdleft;
            if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
                pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
            else        // Bitmap is stored top-to-bottom
                pos = bmpImageoffset + row * rowSize;
            if (bmpFile.position() != pos) { // Need seek?
                bmpFile.seek(pos);
                buffidx = sizeof(sdbuffer); // Force buffer reload
            }

            for (col = 0; col < w; ) {  //pixels in row
                lcdleft = w - col;
                if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
                for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
                    uint16_t color;
                    // Time to read more pixel data?
                    if (buffidx >= sizeof(sdbuffer)) { // Indeed
                        bmpFile.read(sdbuffer, sizeof(sdbuffer));
                        buffidx = 0; // Set index to beginning
                        r = 0;
                    }
                    switch (bmpDepth) {          // Convert pixel from BMP to TFT format
                        case 24:
                            b = sdbuffer[buffidx++];
                            g = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            color = tft.color565(r, g, b);
                            break;
                        case 16:
                            b = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            if (is565)
                                color = (r << 8) | (b);
                            else
                                color = (r << 9) | ((b & 0xE0) << 1) | (b & 0x1F);
                            break;
                        case 1:
                        case 4:
                        case 8:
                            if (r == 0)
                                b = sdbuffer[buffidx++], r = 8;
                            color = palette[(b >> bitshift) & bitmask];
                            r -= bmpDepth;
                            b <<= bmpDepth;
                            break;
                    }
//                    lcdbuffer[lcdidx] = color;
                    lcdbuffer[lcdidx] = (color >> 8)|(color << 8);

                }
                // assumes byte order insted of uint16_t array
#if defined(_TFT_eSPIH_)
                tft.pushColors(lcdbuffer, lcdidx, false);
#else
                tft.pushColors((uint8_t*)lcdbuffer, lcdidx, first); //.kbv
#endif
                first = false;
                col += lcdidx;
            }           // end cols
        }               // end rows
        tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen
        ret = 0;        // good render
    }
    bmpFile.close();
    return (ret);
}

void setup()
{
    static uint16_t ID;
    SPI.begin();
    pinMode(FLASH_CS, OUTPUT);      //Flash chip
    Serial.begin(9600);
    uint8_t buf[16];
    for (uint32_t loc = 0L; loc < 0x100000; loc += 0x10000) {
        read_spi_mem(loc, buf, 16, FLASH_CS);
        hexdump(loc, buf, 16);    
    }
    ID = 0x9488;
#if !defined(_TFT_eSPIH_)
    ID = tft.readID();
#endif
    if (ID == 0x0D3D3) ID = 0x9481;
    tft.begin(ID);
    tft.fillScreen(0x001F);
//    if (tft.height() > tft.width()) tft.setRotation(1);    //LANDSCAPE
    tft.setTextColor(0xFFFF, 0x0000);
}

void loop()
{
    tft.fillScreen(0);
    uint32_t start = millis();
    int ret = showBMP_flash(0L, 5, 20);
    Serial.print("returns ");
    Serial.print(ret);
    Serial.print(" ");
    Serial.print(millis() - start);
    Serial.println("ms");
    delay(4000);
}

