// make all the functions static.
// the linker will discard unused anonymous strings. 

static void read_regs(char *title)
{
    Serial.println(title);
    readReg(0x00, 2, "ID: ILI9320, ILI9325, ILI9335, ...");
    readReg(0x04, 4, "Manufacturer ID");
    readReg(0x09, 5, "Status Register");
    readReg(0x0A, 2, "Get Powsr Mode");
    readReg(0x0C, 2, "Get Pixel Format");
    readReg(0x30, 5, "PTLAR");
    readReg(0x33, 7, "VSCRLDEF");
    readReg(0x61, 2, "RDID1 HX8347-G");
    readReg(0x62, 2, "RDID2 HX8347-G");
    readReg(0x63, 2, "RDID3 HX8347-G");
    readReg(0x64, 2, "RDID1 HX8347-A");
    readReg(0x65, 2, "RDID2 HX8347-A");
    readReg(0x66, 2, "RDID3 HX8347-A");
    readReg(0x67, 2, "RDID Himax HX8347-A");
    readReg(0x70, 2, "Panel Himax HX8347-A");
    readReg(0xA1, 5, "RD_DDB SSD1963");
    readReg(0xB0, 2, "RGB Interface Signal Control");
    readReg(0xB3, 5, "Frame Memory");
    readReg(0xB4, 2, "Frame Mode");
    readReg(0xB6, 5, "Display Control");
    readReg(0xB7, 2, "Entry Mode Set");
    readReg(0xBF, 6, "ILI9481, HX8357-B");
    readReg(0xC0, 9, "Panel Control");
    readReg(0xC1, 4, "Display Timing");
    readReg(0xC5, 2, "Frame Rate");
    readReg(0xC8, 13, "GAMMA");
    readReg(0xCC, 2, "Panel Control");
    readReg(0xD0, 4, "Power Control");
    readReg(0xD1, 4, "VCOM Control");
    readReg(0xD2, 3, "Power Normal");
    readReg(0xD3, 4, "ILI9341, ILI9488");
    readReg(0xDA, 2, "RDID1");
    readReg(0xDB, 2, "RDID2");
    readReg(0xDC, 2, "RDID3");
    readReg(0xE0, 16, "GAMMA-P");
    readReg(0xE1, 16, "GAMMA-N");
    readReg(0xEF, 6, "ILI9327");
    readReg(0xF2, 12, "Adjust Control 2");
    readReg(0xF6, 4, "Interface Control");
}

static void read_61509(char *title)
{
    Serial.println(title);
    readReg(0x0000, 2, "Device Code Read");
    readReg(0x0001, 2, "Device Output Control");
    readReg(0x0002, 2, "LCD Waveform Control");
    readReg(0x0003, 2, "Entry Mode");
    readReg(0x0007, 2, "Display Control 1");
    readReg(0x0008, 2, "Display Control 2");
    readReg(0x0009, 2, "Display Control 3");
    readReg(0x0010, 2, "Panel Interface Control 1");
    readReg(0x0011, 2, "Panel Interface Control 2");
    readReg(0x0012, 2, "Panel Interface Control 3");
    readReg(0x0013, 2, "Panel Interface Control 4");
    readReg(0x0100, 2, "Power Control 1");
    readReg(0x0101, 2, "Power Control 2");
    readReg(0x0102, 2, "Power Control 3");
    readReg(0x0103, 2, "Power Control 4");
    readReg(0x0210, 2, "Horiz SA");
    readReg(0x0211, 2, "Horiz EA");
    readReg(0x0212, 2, "Vert SA");
    readReg(0x0213, 2, "Vert EA");
    readReg(0x0400, 2, "Base Image NL");
    readReg(0x0401, 2, "Base Image Control");
    readReg(0x0404, 2, "Base Image Scroll");
}

static void read_61526(char *title)
{
    Serial.println(title);
    readReg(0x04, 4, "Manufacturer ID");
    readReg(0x0A, 2, "Get Power Mode");
    readReg(0x0B, 2, "Get Address Mode");
    readReg(0x0C, 2, "Get Pixel Format");
    readReg(0x0D, 2, "Get Display Mode");
    readReg(0x0E, 2, "Get Signal Mode");
    readReg(0x0F, 2, "Get Diagnostic Result");
    readReg(0xA1, 6, "RD_DDB Start");
    readReg(0xB0, 3, "Command Access Protect");
    readReg(0xB3, 6, "Frame Memory");
    readReg(0xB4, 2, "Frame Mode");
    readReg(0xBF, 5, "Device Code Read");
    readReg(0xC0, 9, "Panel Control");
    readReg(0xC1, 6, "Display Timing Normal");
    readReg(0xC3, 6, "Display Timing Idle");
    readReg(0xC4, 6, "Timing setting");
    readReg(0xC8, 23, "GAMMA");
    readReg(0xC9, 23, "GAMMA");
    readReg(0xCA, 23, "GAMMA");
    readReg(0xD0, 7, "Power Control");
    readReg(0xD1, 4, "VCOM Control");
    readReg(0xD2, 3, "Power Normal");
    readReg(0xD4, 3, "Power Idle");    
    readReg(0xE0, 4, "NVM Access");    
    readReg(0xE1, 2, "set_DDB");    
    readReg(0xE2, 2, "NVM Load Control");    
}

static void read_fk(char *title)
{
    Serial.println(title);
    readReg(0x0A, 2, "Get Powsr Mode");
    readReg(0x0B, 2, "Get Address Mode");
    readReg(0x0C, 2, "Get Pixel Format");
    readReg(0x0D, 2, "Get Display Mode");
    readReg(0x0E, 2, "Get Signal Mode");
    readReg(0x0F, 2, "Get Diagnostic Result");
    readReg(0xA1, 6, "RD_DDB Start");
    readReg(0xB0, 3, "Command Access Protect");
    readReg(0xBF, 5, "Device Code Read");
    readReg(0xF0, 4, "f.k.");    
    readReg(0xF2, 18, "f.k.");    
    readReg(0xF3, 2, "f.k.");    
    readReg(0xF4, 14, "f.k.");    
    readReg(0xF5, 11, "f.k.");    
    readReg(0xF6, 10, "f.k.");    
    readReg(0xF7, 4, "f.k.");    
    readReg(0xF8, 2, "f.k.");    
    readReg(0xF9, 2, "f.k.");    
    readReg(0xFA, 12, "f.k.");    
    readReg(0xFB, 2, "f.k.");    
    readReg(0xFC, 2, "f.k.");    
    readReg(0xFD, 12, "f.k.");    
    readReg(0xFE, 2, "f.k.");    
}

static void read_9327(char *title)
{
    Serial.println(title);
    readReg(0x0A, 2, "Get Power Mode");
    readReg(0x0B, 2, "Get Address Mode");
    readReg(0x0C, 2, "Get Pixel Format");
    readReg(0x0D, 2, "Get Display Mode");
    readReg(0x0E, 2, "Get Signal Mode");
    readReg(0x0F, 2, "Get Diagnostic Result");
    readReg(0xA1, 6, "RD_DDB Start");
    readReg(0xB0, 2, "Command Access Protect");
    readReg(0xB1, 2, "Low Power Control");
    readReg(0xB3, 5, "Frame Memory");
    readReg(0xB4, 2, "Frame Mode");
    readReg(0xB5, 2, "Sub Panel Control");
    readReg(0xBF, 2, "Backlight Control");
    readReg(0xC0, 7, "Panel Control");
    readReg(0xC1, 4, "Display Timing Normal");
    readReg(0xC3, 4, "Display Timing Idle");
    readReg(0xC4, 2, "VCOM Timing");
    readReg(0xC5, 2, "Frame Rate");
    readReg(0xC6, 2, "Interface Control");
    readReg(0xC8, 13, "GAMMA");
    readReg(0xC9, 13, "GAMMA Red/Blue");
    readReg(0xCC, 2, "Panel Control");
    readReg(0xD0, 4, "Power Control");
    readReg(0xD1, 4, "VCOM Control");
    readReg(0xD2, 3, "Power Normal");
    readReg(0xD3, 3, "Power Partial");
    readReg(0xD4, 3, "Power Idle");    
    readReg(0xEF, 6, "Device Code Read");
}

static void read_9338(char *title)
{
    Serial.println(title);
    readReg(0x04, 4, "Manufacturer ID");
//    readReg(0x05, 2, "DSI errors");
    readReg(0x09, 5, "Status Register");
    readReg(0x0A, 2, "Get Powsr Mode");
    readReg(0x0B, 2, "Get Address Mode");
    readReg(0x0C, 2, "Get Pixel Format");
    readReg(0x0D, 2, "Get Display Mode");
    readReg(0x0E, 2, "Get Signal Mode");
    readReg(0x0F, 2, "Get Diagnostic Result");
    readReg(0xB0, 2, "Interface Mode");
    readReg(0xB1, 3, "Frame Rate Normal");
    readReg(0xB2, 3, "Frame Rate Idle");
    readReg(0xB3, 3, "Frame Rate Partial");
    readReg(0xB4, 3, "Inversion Control");
    readReg(0xB5, 5, "Blanking Porch");
    readReg(0xB6, 4, "Display Control");
    readReg(0xB7, 2, "Entry Mode Set");
    readReg(0xC0, 2, "Power Control 1");
    readReg(0xC1, 2, "Power Control 2");
    readReg(0xC2, 2, "Power Control 3");
    readReg(0xC3, 2, "Power Control 4");
    readReg(0xC4, 2, "Power Control 5");
    readReg(0xC5, 3, "VCOM Control 1");
    readReg(0xC7, 2, "VCOM Control 2");

    readReg(0xD0, 3, "NVM Write");
    readReg(0xD1, 4, "NVM key");

    readReg(0xD2, 3, "NVM Status");
    readReg(0xD3, 4, "RDID4");
    readReg(0xDA, 2, "RDID1");
    readReg(0xDB, 2, "RDID2");
    readReg(0xDC, 2, "RDID3");
    readReg(0xE0, 16, "GAMMA-P");
    readReg(0xE1, 16, "GAMMA-N");
    readReg(0xF2, 2, "GAMMA-3G");
    readReg(0xF6, 4, "Interface Control");
}

static void read_9481(char *title)
{
    Serial.println(title);
    readReg(0x0A, 2, "Get Powsr Mode");
    readReg(0x0B, 2, "Get Address Mode");
    readReg(0x0C, 2, "Get Pixel Format");
    readReg(0x0D, 2, "Get Display Mode");
    readReg(0x0E, 2, "Get Signal Mode");
    readReg(0x0F, 2, "Get Diagnostic Result");
    readReg(0xA1, 6, "RD_DDB Start");
    readReg(0xB0, 2, "Command Access Protect");
    readReg(0xB3, 5, "Frame Memory");
    readReg(0xB4, 2, "Frame Mode");
    readReg(0xBF, 6, "Device Code Read");
    readReg(0xC0, 6, "Panel Control");
    readReg(0xC1, 4, "Display Timing Normal");
    readReg(0xC2, 4, "Display Timing Partial");
    readReg(0xC3, 4, "Display Timing Idle");
    readReg(0xC5, 2, "Frame Rate");
    readReg(0xC6, 2, "Interface Control");
    readReg(0xC8, 13, "GAMMA");
    readReg(0xCC, 2, "Panel Control");
    readReg(0xD0, 4, "Power Control");
    readReg(0xD1, 4, "VCOM Control");
    readReg(0xD2, 3, "Power Normal");
    readReg(0xD3, 3, "Power Partial");
    readReg(0xD4, 3, "Power Idle");    
    readReg(0xE4, 4, "? SETCABC(HX) (f.k. ILI)");    
    readReg(0xE9, 4, "? Set Panel Related(HX)");    
    readReg(0xEE, 5, "? Set EQ Function(HX)");    
    readReg(0xF0, 4, "f.k (ILI)");    
    readReg(0xF1, 2, "f.k");    
    readReg(0xF2, 4, "f.k");    
    readReg(0xF3, 4, "f.k (ILI)");    
    readReg(0xF4, 4, "f.k");    
    readReg(0xF5, 2, "f.k");    
    readReg(0xF6, 2, "f.k");    
    readReg(0xF7, 4, "f.k");    
    readReg(0xF8, 3, "f.k");    
    readReg(0xF9, 2, "f.k");    
}

static void read_9486(char *title)
{
    Serial.println(title);
    readReg(0x04, 4, "Manufacturer ID");
    readReg(0x05, 2, "DSI errors");
    readReg(0x09, 5, "Status Register");
    readReg(0x0A, 2, "Get Powsr Mode");
    readReg(0x0B, 2, "Get Address Mode");
    readReg(0x0C, 2, "Get Pixel Format");
    readReg(0x0D, 2, "Get Display Mode");
    readReg(0x0E, 2, "Get Signal Mode");
    readReg(0x0F, 2, "Get Diagnostic Result");
    readReg(0xB0, 2, "Interface Mode");
    readReg(0xB1, 3, "Frame Rate Normal");
    readReg(0xB2, 3, "Frame Rate Idle");
    readReg(0xB3, 3, "Frame Rate Partial");
    readReg(0xB4, 2, "Inversion Control");
    readReg(0xB5, 5, "Blanking Porch");
    readReg(0xB6, 4, "Display Control");
    readReg(0xB7, 2, "Entry Mode Set");
    readReg(0xBF, 6, "NON-Ilitek Device Code Read");
    readReg(0xC0, 3, "Power Control 1");
    readReg(0xC1, 3, "Power Control 2");
    readReg(0xC2, 2, "Power Control 3");
    readReg(0xC3, 2, "Power Control 4");
    readReg(0xC4, 2, "Power Control 5");
    readReg(0xC5, 5, "VCOM Control 1");
/*
    readReg(0xC6, 3, "CABC Control 1");
    readReg(0xC8, 3, "CABC Control 2");
    readReg(0xC9, 2, "CABC Control 3");
    readReg(0xCA, 2, "CABC Control 4");
    readReg(0xCB, 2, "CABC Control 5");
    readReg(0xCC, 2, "CABC Control 6");
    readReg(0xCD, 2, "CABC Control 7");
    readReg(0xCE, 2, "CABC Control 8");
    readReg(0xCF, 2, "CABC Control 9");
    readReg(0xD0, 3, "NVM Write");
    readReg(0xD1, 4, "NVM key");
    readReg(0xD2, 5, "NVM Status");
*/
    readReg(0xD3, 4, "RDID4");
    readReg(0xDA, 2, "RDID1");
    readReg(0xDB, 2, "RDID2");
    readReg(0xDC, 2, "RDID3");
    readReg(0xE0, 16, "GAMMA-P");
    readReg(0xE1, 16, "GAMMA-N");
    readReg(0xF2, 10, "Adjust 2");
    readReg(0xF7, 5, "Adjust 3");
    readReg(0xF8, 3, "Adjust 4");
    readReg(0xF9, 3, "Adjust 5");
    readReg(0xFB, 2, "SPI Read");
    readReg(0xFC, 3, "Adjust 6");
}

