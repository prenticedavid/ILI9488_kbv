#define USE_ILI9341
#include "ILI9488_kbv.h"
#include "serial_kbv.h"

#if defined(USE_ILI9341)
ILI9488_kbv::ILI9488_kbv():Adafruit_GFX(240, 320){}
#else
ILI9488_kbv::ILI9488_kbv():Adafruit_GFX(320, 480){}
#endif

static uint8_t done_reset;

void ILI9488_kbv::reset(void)
{
    done_reset = 1;
    INIT();
    CS_IDLE;
    RESET_IDLE;
	wait_ms(50);
	RESET_ACTIVE;
	wait_ms(100);
	RESET_IDLE;
	wait_ms(100);
}

void ILI9488_kbv::pushCommand(uint16_t cmd, uint8_t * block, int8_t N)
{
    CS_ACTIVE;
    WriteCmd(cmd);
	write8_block(block, N);
    CS_IDLE;
}

#define ILI9488_CMD_NOP                             0x00
#define ILI9488_CMD_SOFTWARE_RESET                  0x01
#define ILI9488_CMD_READ_DISP_ID                    0x04
#define ILI9488_CMD_READ_DISP_STATUS                0x09
#define ILI9488_CMD_READ_DISP_MADCTRL               0x0B
#define ILI9488_CMD_READ_DISP_PIXEL_FORMAT          0x0C
#define ILI9488_CMD_READ_DISP_IMAGE_FORMAT          0x0D
#define ILI9488_CMD_READ_DISP_SIGNAL_MODE           0x0E
#define ILI9488_CMD_READ_DISP_SELF_DIAGNOSTIC       0x0F
#define ILI9488_CMD_ENTER_SLEEP_MODE                0x10
#define ILI9488_CMD_SLEEP_OUT                       0x11
#define ILI9488_CMD_PARTIAL_MODE_ON                 0x12
#define ILI9488_CMD_NORMAL_DISP_MODE_ON             0x13
#define ILI9488_CMD_DISP_INVERSION_OFF              0x20
#define ILI9488_CMD_DISP_INVERSION_ON               0x21
#define ILI9488_CMD_GAMMA_SET                       0x26
#define ILI9488_CMD_DISPLAY_OFF                     0x28
#define ILI9488_CMD_DISPLAY_ON                      0x29
#define ILI9488_CMD_COLUMN_ADDRESS_SET              0x2A
#define ILI9488_CMD_PAGE_ADDRESS_SET                0x2B
#define ILI9488_CMD_MEMORY_WRITE                    0x2C
#define ILI9488_CMD_MEMORY_READ                     0x2E
#define ILI9488_CMD_PARTIAL_AREA                    0x30
#define ILI9488_CMD_VERT_SCROLL_DEFINITION          0x33
#define ILI9488_CMD_TEARING_EFFECT_LINE_OFF         0x34
#define ILI9488_CMD_TEARING_EFFECT_LINE_ON          0x35
#define ILI9488_CMD_MEMORY_ACCESS_CONTROL           0x36
#define ILI9488_CMD_VERT_SCROLL_START_ADDRESS       0x37
#define ILI9488_CMD_IDLE_MODE_OFF                   0x38
#define ILI9488_CMD_IDLE_MODE_ON                    0x39
#define ILI9488_CMD_COLMOD_PIXEL_FORMAT_SET         0x3A
#define ILI9488_CMD_WRITE_MEMORY_CONTINUE           0x3C
#define ILI9488_CMD_READ_MEMORY_CONTINUE            0x3E
#define ILI9488_CMD_SET_TEAR_SCANLINE               0x44
#define ILI9488_CMD_GET_SCANLINE                    0x45

static uint8_t readReg8(uint8_t reg, uint8_t dat)
{
    uint8_t ret;
    CD_COMMAND;
    CS_ACTIVE;
    xchg8(reg);
	CD_DATA;                    //should do a flush()
	ret = xchg8(dat);           //only safe to read @ SCK=16MHz
    CS_IDLE;
    return ret;
}

uint8_t ILI9488_kbv::readcommand8(uint8_t reg, uint8_t idx)         //this is the same as Adafruit_ILI9488
{
    readReg8(0xFB, 0x80 | idx);   //SPI_READ, SPI_READ_EN
    return readReg8(reg, 0xFF); 
}
    
uint16_t ILI9488_kbv::readID(void)                          //{ return 0x9341; }
{
    if (!done_reset) reset();
	return (readcommand8(0xD3, 2) << 8) | readcommand8(0xD3, 3);
}
    
uint16_t ILI9488_kbv::readReg(uint16_t reg, uint8_t idx)     //note that this reads pairs of data bytes
{
    uint8_t h, l;
    idx <<= 1;
    h = readcommand8(reg, idx);
    l = readcommand8(reg, idx + 1);
    return (h << 8) | l;
}

uint32_t ILI9488_kbv::readReg32(uint16_t reg)
{
	uint32_t ret;
    for (uint8_t idx = 0; idx < 4; idx++) {
		ret <<= 8;
		ret |= readcommand8(reg, idx);
	}
	return ret;
}

int16_t ILI9488_kbv::readGRAM(int16_t x, int16_t y, uint16_t * block, int16_t w, int16_t h)
{
    uint8_t r, g, b;
	  int16_t n = w * h;    // we are NEVER going to read > 32k pixels at once
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    CS_ACTIVE;
    WriteCmd(ILI9488_CMD_MEMORY_READ);

    // needs 1 dummy read
    r = xchg8(0xFF);
    while (n-- > 0) {
        r = xchg8(0xFF);
        g = xchg8(0xFF);
        b = xchg8(0xFF);
		*block++ = color565(r, g, b);
    }
    CS_IDLE;
    setAddrWindow(0, 0, width() - 1, height() - 1);
    return 0;
}

void ILI9488_kbv::setRotation(uint8_t r)
{
    uint8_t mac = 0x00;
    Adafruit_GFX::setRotation(r & 3);
    switch (rotation) {
    case 0:
        mac = 0x08;
        break;
    case 1:        //LANDSCAPE 90 degrees 
        mac = 0x68;
        break;
    case 2:
        mac = 0xD8;
        break;
    case 3:
        mac = 0xB8;
        break;
    }
    pushCommand(ILI9488_CMD_MEMORY_ACCESS_CONTROL, &mac, 1);
}

void ILI9488_kbv::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    // ILI934X just plots at edge if you try to write outside of the box:
    if (x < 0 || y < 0 || x >= width() || y >= height())
        return;
    CS_ACTIVE;
    WriteCmd(ILI9488_CMD_COLUMN_ADDRESS_SET);
    write16(x);
    WriteCmd(ILI9488_CMD_PAGE_ADDRESS_SET);
    write16(y);
    WriteCmd(ILI9488_CMD_MEMORY_WRITE);
    write24(color);
    CS_IDLE;
}

void ILI9488_kbv::setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1)
{
    CS_ACTIVE;
    WriteCmd(ILI9488_CMD_COLUMN_ADDRESS_SET);
    write16(x);
    write16(x1);
    WriteCmd(ILI9488_CMD_PAGE_ADDRESS_SET);
    write16(y);
    write16(y1);
    CS_IDLE;
}

void ILI9488_kbv::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    int16_t end;
    if (w < 0) {
        w = -w;
        x -= w;
    }                           //+ve w
    end = x + w;
    if (x < 0)
        x = 0;
    if (end > width())
        end = width();
    w = end - x;
    if (h < 0) {
        h = -h;
        y -= h;
    }                           //+ve h
    end = y + h;
    if (y < 0)
        y = 0;
    if (end > height())
        end = height();
    h = end - y;
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    CS_ACTIVE;
    WriteCmd(ILI9488_CMD_MEMORY_WRITE);
	if (h > w) { end = h; h = w; w = end; } 
    while (h-- > 0) {
        write24_N(color, w);
    }
    CS_IDLE;
    setAddrWindow(0, 0, width() - 1, height() - 1);
}

void ILI9488_kbv::pushColors(uint16_t * block, int16_t n, bool first)
{
    uint16_t color;
    CS_ACTIVE;
    if (first) {
        WriteCmd(ILI9488_CMD_MEMORY_WRITE);
    }
    while (n-- > 0) {
        color = *block++;
        write24(color);
    }
    CS_IDLE;
}

void ILI9488_kbv::pushColors(uint8_t * block, int16_t n, bool first)
{
    uint16_t color;
    uint8_t h, l;
    CS_ACTIVE;
    if (first) {
        WriteCmd(ILI9488_CMD_MEMORY_WRITE);
    }
    CD_DATA;
    while (n-- > 0) {
        h = (*block++);
        l = (*block++);
        color = (h << 8) | l;
        write24(color);
    }
    CS_IDLE;
}

void ILI9488_kbv::pushColors(const uint8_t * block, int16_t n, bool first, bool bigend)
{
    uint16_t color;
	uint8_t h, l;
	CS_ACTIVE;
    if (first) {
        WriteCmd(ILI9488_CMD_MEMORY_WRITE);
    }
    CD_DATA;
    while (n-- > 0) {
        l = pgm_read_byte(block++);
        h = pgm_read_byte(block++);
        color = (bigend) ? (l << 8 ) | h : (h << 8) | l;
		write24(color);
    }
    CS_IDLE;
}

void ILI9488_kbv::invertDisplay(boolean i)
{
    pushCommand(i ? ILI9488_CMD_DISP_INVERSION_ON : ILI9488_CMD_DISP_INVERSION_OFF, NULL, 0);
}
    
void ILI9488_kbv::vertScroll(int16_t top, int16_t scrollines, int16_t offset)
{
    int16_t bfa = HEIGHT - top - scrollines;  // bottom fixed area
    int16_t vsp;
    vsp = top + offset;  // vertical start position
    if (offset < 0)
        vsp += scrollines;          //keep in unsigned range
	CS_ACTIVE;
    WriteCmd( 0x0033);
	write16(top);        //TOP
	write16(scrollines); //VSA
    write16(bfa);        //BFA

    WriteCmd(0x0037)
	write16(vsp);        //VLSP
    CS_IDLE;	
}

#define TFTLCD_DELAY8 0xFF

const uint8_t PROGMEM ILI9488_regValues_kbv[] = {
    //  (COMMAND_BYTE), n, data_bytes....
            0x01, 0,            //Soft Reset
            TFTLCD_DELAY8, 150,  // .kbv will power up with ONLY reset, sleep out, display on
            0x28, 0,            //Display Off
            0x3A, 1, 0x66,      //Pixel read=666, write=666
            0xC0, 2, 0x10, 0x10,        //Power Control 1 [0E 0E]
            0xC1, 1, 0x41,      //Power Control 2 [43]
            0xC5, 4, 0x00, 0x22, 0x80, 0x40,    //VCOM  Control 1 [00 40 00 40]
            0x36, 1, 0x68,      //Memory Access [00]
            0xB0, 1, 0x00,      //Interface     [00]
            0xB1, 2, 0xB0, 0x11,        //Frame Rate Control [B0 11]
            0xB4, 1, 0x02,      //Inversion Control [02]
            0xB6, 3, 0x02, 0x02, 0x3B,  // Display Function Control [02 02 3B] .kbv NL=480
            0xB7, 1, 0xC6,      //Entry Mode      [06]
            0x3A, 1, 0x55,      //Interlace Pixel Format [XX]
            0xF7, 4, 0xA9, 0x51, 0x2C, 0x82,    //Adjustment Control 3 [A9 51 2C 82]
			0x11, 0,            //Sleep Out
            TFTLCD_DELAY8, 150,
            0x29, 0,            //Display On
};

#if defined(USE_ILI9341)
		static const uint8_t ILI9341_regValues_2_4[] PROGMEM = {   // BOE 2.4"
			0x01, 0,            // software reset
			TFTLCD_DELAY8, 50,   // .kbv
			0xCF, 3, 0x00, 0x81, 0x30,  //Power Control B [00 81 30]
			0xED, 4, 0x64, 0x03, 0x12, 0x81,    //Power On Seq [55 01 23 01]
			0xE8, 3, 0x85, 0x10, 0x78,  //Driver Timing A [04 11 7A]
			0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,      //Power Control A [39 2C 00 34 02]
			0xF7, 1, 0x20,      //Pump Ratio [10]
			0xEA, 2, 0x00, 0x00,        //Driver Timing B [66 00]
			0xB1, 2, 0x00, 0x1B,        //Frame Control [00 1B]
            0xB6, 3, 0x0A, 0xA2, 0x27, //Display Function [0A 82 27 XX]    .kbv SS=1
			0xB4, 1, 0x00,      //Inversion Control [02] .kbv NLA=1, NLB=1, NLC=1
			0xC0, 1, 0x21,      //Power Control 1 [26]
			0xC1, 1, 0x11,      //Power Control 2 [00]
			0xC5, 2, 0x3F, 0x3C,        //VCOM 1 [31 3C]
			0xC7, 1, 0xB5,      //VCOM 2 [C0]
			0x36, 1, 0x48,      //Memory Access [00]
			0xF2, 1, 0x00,      //Enable 3G [02]
			0x26, 1, 0x01,      //Gamma Set [01]
			0xE0, 15, 0x0f, 0x26, 0x24, 0x0b, 0x0e, 0x09, 0x54, 0xa8, 0x46, 0x0c, 0x17, 0x09, 0x0f, 0x07, 0x00,
			0xE1, 15, 0x00, 0x19, 0x1b, 0x04, 0x10, 0x07, 0x2a, 0x47, 0x39, 0x03, 0x06, 0x06, 0x30, 0x38, 0x0f,
			0x11, 0,            //Sleep Out
			TFTLCD_DELAY8, 150,
			0x29, 0,            //Display On
			0x3A, 1, 0x66,      //Pixel Format [66]
		};
#define tableNNNN ILI9341_regValues_2_4
#else
#define tableNNNN ILI9488_regValues_kbv
#endif

void ILI9488_kbv::begin(uint16_t ID)
{
    _lcd_ID = ID;
    uint8_t *p = (uint8_t *) tableNNNN;
    int16_t size = sizeof(tableNNNN);
    reset();
    while (size > 0) {
	    uint8_t cmd = pgm_read_byte(p++);
	    uint8_t len = pgm_read_byte(p++);
	    if (cmd == TFTLCD_DELAY8) {
		    delay(len);
		    len = 0;
		} else {
		    CS_ACTIVE;
		    WriteCmd(cmd);
		    for (uint8_t d = 0; d < len; d++) {
			    uint8_t x = pgm_read_byte(p++);
			    xchg8(x);
		    }
		    CS_IDLE;
	    }
	    size -= len + 2;
    }
    setRotation(0);             //PORTRAIT
}
