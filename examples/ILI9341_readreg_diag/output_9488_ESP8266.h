Read registers 8-bit SPI with MISO (+ DC)
ILI9486 / 1LI9488
 reg(0x04) 54 80 66 00    Manufacturer ID
 reg(0x05) 00 00    DSI errors
 reg(0x09) 00 61 00 00 00   Status Register
 reg(0x0A) 08 00    Get Powsr Mode
 reg(0x0B) 00 00    Get Address Mode
 reg(0x0C) 06 00    Get Pixel Format
 reg(0x0D) 00 00    Get Display Mode
 reg(0x0E) 00 00    Get Signal Mode
 reg(0x0F) 00 00    Get Diagnostic Result
 reg(0xB0) 00 00    Interface Mode
 reg(0xB1) 11 A0 11 Frame Rate Normal
 reg(0xB2) 11 00 11 Frame Rate Idle
 reg(0xB3) 11 00 11 Frame Rate Partial
 reg(0xB4) 02 02    Inversion Control
 reg(0xB5) 04 02 02 0A 04   Blanking Porch
 reg(0xB6) 3B 02 02 3B  Display Control
 reg(0xB7) 06 06    Entry Mode Set
 reg(0xBF) 00 00 00 00 00 00    NON-Ilitek Device Code Read
 reg(0xC0) 0E 0E 0E Power Control 1
 reg(0xC1) 08 44 08 Power Control 2
 reg(0xC2) 33 33    Power Control 3
 reg(0xC3) 33 33    Power Control 4
 reg(0xC4) 33 33    Power Control 5
 reg(0xC5) 40 00 40 00 40   VCOM Control 1
 reg(0xD3) 88 00 94 88  RDID4
 reg(0xDA) 54 54    RDID1
 reg(0xDB) 80 80    RDID2
 reg(0xDC) 66 66    RDID3
 reg(0xE0) 0F 00 07 0C 05 13 09 36 AA 46 09 10 0D 1A 1E 0F  GAMMA-P
 reg(0xE1) 0F 00 20 23 04 10 06 37 56 49 04 0C 0A 33 37 0F  GAMMA-N
 reg(0xF2) 88 58 04 12 02 22 22 FF 0A 90    Adjust 2
 reg(0xF7) 09 A9 51 2C 82   Adjust 3
 reg(0xF8) 00 21 04 Adjust 4
 reg(0xF9) 00 00 00 Adjust 5
 reg(0xFB) FF FF    SPI Read
 reg(0xFC) 05 00 05 Adjust 6

