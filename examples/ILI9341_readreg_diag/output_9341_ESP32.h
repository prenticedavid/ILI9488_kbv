Read registers 8-bit SPI with MISO (+ DC)
ILI9302 / ILI9329 / ILI9338 / ILI9341
 reg(0x04) 00 00 00 FF    Manufacturer ID
 reg(0x09) 00 61 00 00 FF   Status Register
 reg(0x0A) 08 FF    Get Powsr Mode
 reg(0x0B) 00 FF    Get Address Mode
 reg(0x0C) 06 FF    Get Pixel Format
 reg(0x0D) 00 FF    Get Display Mode
 reg(0x0E) 00 FF    Get Signal Mode
 reg(0x0F) 00 FF    Get Diagnostic Result
 reg(0xB0) 00 00    Interface Mode
 reg(0xB1) 1B 00 1B Frame Rate Normal
 reg(0xB2) 1B 00 1B Frame Rate Idle
 reg(0xB3) 1B 00 1B Frame Rate Partial
 reg(0xB4) 00 02 00 Inversion Control
 reg(0xB5) 14 02 02 0A 14   Blanking Porch
 reg(0xB6) 00 0A 82 27  Display Control
 reg(0xB7) 06 06    Entry Mode Set
 reg(0xC0) 00 21    Power Control 1
 reg(0xC1) 10 10    Power Control 2
 reg(0xC2) 00 00    Power Control 3
 reg(0xC3) 00 00    Power Control 4
 reg(0xC4) 00 00    Power Control 5
 reg(0xC5) 3C 31 3C VCOM Control 1
 reg(0xC7) C0 C0    VCOM Control 2
 reg(0xD0) 00 00 00 NVM Write
 reg(0xD1) FF FF FF FF  NVM key
 reg(0xD2) 03 00 00 NVM Status
 reg(0xD3) 41 00 93 41  RDID4
 reg(0xDA) 00 00    RDID1
 reg(0xDB) 00 00    RDID2
 reg(0xDC) 00 00    RDID3
 reg(0xE0) 00 0F 16 14 0A 0D 06 43 75 33 06 0E 00 0C 09 08  GAMMA-P
 reg(0xE1) 00 08 2B 2D 04 10 04 3E 24 4E 04 0F 0E 35 38 0F  GAMMA-N
 reg(0xF2) 02 02    GAMMA-3G
 reg(0xF6) 00 01 00 00  Interface Control

