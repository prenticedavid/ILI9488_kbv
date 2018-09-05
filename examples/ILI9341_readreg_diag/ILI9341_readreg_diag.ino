/*
Serial.println("1. select correct defines for your board wiring");
Serial.println("2. select correct interface type");
Serial.println("3. select appropriate report function");
Serial.println("");
Serial.println("certain registers read first byte e.g. 0x00-0x0F, 0xDA-0xDC");
Serial.println("other multi-argument registers read first byte as dummy");
Serial.println("");
*/

#if defined(ESP32)
#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_SCK  18
#define TFT_SS   5
#define TFT_DC   13     //DC=7 for HX8347
#define TFT_RESET 12   //Backlight on HX8347
#elif defined(ESP8266)
#define TFT_MOSI D11
#define TFT_MISO D12
#define TFT_SCK  D13
#define TFT_SS   D10
#define TFT_DC   D9     //DC=7 for HX8347
#define TFT_RESET D8   //Backlight on HX8347
#else                   //suits Uno, Nucleo, ...
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_SCK  13
#define TFT_SS   10
#define TFT_DC  (9)     //DC=7 for HX8347
#define TFT_RESET (8)   //Backlight on HX8347
#endif

#define NINEBITS  (1<<0)
#define SDA_INPUT (1<<1)
#define HAS_MISO  (1<<2)
#define IS_9341   (1<<3)
#define IS_9481   (1<<4)
#define IS_9486   (1<<5)
const char *chip = "controller";
//char interface = 0;    //regular ST7735, ILI9163
//char interface = HAS_MISO | IS_9341;  //ILI9341 SPI
//char interface = HAS_MISO | IS_9481;  //ILI9481 SPI
char interface = HAS_MISO | IS_9486;  //ILI9486 SPI
//char interface = HAS_MISO;  //regular SPI
//char interface = NINEBITS | SDA_INPUT | IS_9481;  //ILI9481 3-wire SPI bidirectional pin

void setup() {
    // put your setup code here, to run once:
    uint32_t ID = 0;
    Serial.begin(9600);

    Serial.println("");
    Serial.println("1. select correct defines for your board wiring");
    Serial.println("2. select correct interface type");
    Serial.println("3. select appropriate report function");
    Serial.println("");
    Serial.println("certain registers read first byte e.g. 0x00-0x0F, 0xDA-0xDC");
    Serial.println("other multi-argument registers read first byte as dummy");
    Serial.println("");
    
    if (interface & HAS_MISO)
        Serial.println("Read registers 8-bit SPI with MISO (+ DC)");
    else if (interface & NINEBITS)
        Serial.println("Bi-directional Read registers 9-bit SPI");
    else
        Serial.println("Bi-directional Read registers SDA (+ DC)");
    digitalWrite(TFT_SS, HIGH);
    //    digitalWrite(TFT_SCK, HIGH);
    pinMode(TFT_SS, OUTPUT);
    pinMode(TFT_SCK, OUTPUT);
    pinMode(TFT_MOSI, OUTPUT);
    pinMode(TFT_MISO, INPUT);
    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_RESET, OUTPUT);
    digitalWrite(TFT_RESET, HIGH);
    digitalWrite(TFT_RESET, LOW);   //Hardware Reset
    delay(50);
    digitalWrite(TFT_RESET, HIGH);
    writeblock(0x01, NULL, 0);            //Software Reset
    delay(100);
    ID = readwrite8(0x04, 24, 1);
    if (ID == 0x7C89F0uL) chip = "ST7735S";
    if (ID == 0x548066uL) chip = "ILI9163C";
    ID &= 0xFF0000;
    if (ID == 0x5C0000) chip = "ST7735";
    //    read_7735(chip);
    //    read_regs("diagnose any controller");
    //    read_61509("R61509 / ILI9326");
    //    read_9327("ILI9327");
    read_9338("ILI9302 / ILI9329 / ILI9338 / ILI9341");
    if (interface & NINEBITS) find_sekret();
    //    read_9481("ILI9481 / HX8357 / R61581");
    //    read_9486("ILI9486 / 1LI9488");
}

void read_7735(char *title)
{
    Serial.println(title);
    /*
        Serial.println("Scan all registers for any response");
        for (uint8_t reg = 0; reg < 0xFF; reg++) {
            if (readwrite8(reg, 32, 0) != 0xFFFFFFFF)
                show7735(reg, 32, 0);
        }
    */
    Serial.println("data sheet specific calls");
    show7735(0x04, 3, 1);  //RDDID
    show7735(0x09, 4, 1);  //RDDSTATUS
    show7735(0x0A, 1, 0);
    show7735(0x0B, 1, 0);   //RDDMADCTL
    show7735(0x0C, 1, 0);   //RDDCOLMOD
    show7735(0x0D, 1, 0);
    show7735(0x0E, 1, 0);
    show7735(0x0F, 1, 0);
    show7735(0x2E, 3, 8);  //readGRAM
    show7735(0xA1, 4, 1);  //ILI9481 DDB_Start
    show7735(0xBF, 4, 1);  //ILI9481 ID
    show7735(0xD3, 4, 1);  //RDID4 according to data sheet
    show7735(0xDA, 1, 0);   //RDID1
    show7735(0xDB, 1, 0);   //RDID2
    show7735(0xDC, 1, 0);   //RDID3
}

uint32_t readwrite8(uint8_t cmd, uint8_t bits, uint8_t dummy)
{
    digitalWrite(TFT_SS, LOW);
    write9(cmd, 0);
    uint32_t ret = read8(bits, dummy);
    digitalWrite(TFT_SS, HIGH);
    return ret;
}

void write9(uint8_t val, uint8_t dc)
{
    pinMode(TFT_MOSI, OUTPUT);
    digitalWrite(TFT_DC, dc);
    if (interface & NINEBITS) {
        digitalWrite(TFT_MOSI, dc);
        digitalWrite(TFT_SCK, HIGH);
        digitalWrite(TFT_SCK, LOW);
    }
    for (int i = 0; i < 8; i++) {   //send command
        digitalWrite(TFT_MOSI, (val & 0x80) != 0);
        digitalWrite(TFT_SCK, HIGH);
        digitalWrite(TFT_SCK, LOW);
        val <<= 1;
    }
}

void writeblock(uint8_t cmd, uint8_t *block, int8_t N)
{
    uint8_t val = cmd;
    digitalWrite(TFT_SS, LOW);
    write9(cmd, 0);
    while (N-- > 0) write9(*block++, 1);
    digitalWrite(TFT_SS, HIGH);
}

uint32_t read8(uint8_t bits, uint8_t dummy)
{
    uint32_t ret = 0;
    uint8_t SDAPIN = (interface & HAS_MISO) ? TFT_MISO : TFT_MOSI;
    pinMode(SDAPIN, INPUT_PULLUP);
    digitalWrite(TFT_DC, HIGH);
    for (int i = 0; i < dummy; i++) {  //any dummy clocks
        digitalWrite(TFT_SCK, HIGH);
        digitalWrite(TFT_SCK, LOW);
    }
    for (int i = 0; i < bits; i++) {  // read results
        ret <<= 1;
        if (digitalRead(SDAPIN)) ret |= 1;;
        digitalWrite(TFT_SCK, HIGH);
        digitalWrite(TFT_SCK, LOW);
    }
    return ret;
}

void show7735(uint8_t reg, uint8_t bytes, uint8_t dummy)
{
    uint32_t val, mask = 0x10000000;
    uint8_t bits = bytes * 8;
    for (uint8_t wid = 32; wid > bits; wid -= 4) mask >>= 4;
    writeblock(0x01, NULL, 0);   //software reset first
    delay(100);
    val = readwrite8(reg, bits, dummy);

    Serial.print(chip);
    Serial.print(" reg(0x");
    if (reg < 0x10) Serial.print("0");
    Serial.print(reg , HEX);
    Serial.print(") = 0x");
    while (val < mask) Serial.print("0"), mask >>= 4;
    if (val) Serial.println(val, HEX);
    else Serial.println();
}

void printhex(uint8_t val)
{
    if (val < 0x10) Serial.print("0");
    Serial.print(val, HEX);
}

// Most SPI chips should have some method for reading registers
// ILI9341, 9486 have SPI Read Command Setting (D9h,FBh)
// HX8357-C, -D have SETREADINDEX: set SPI read index (FEh)
// R61581 has Read Mode In (EFh)
// ILI9341: (0xD9) [idx] (reg) [xx] [read]
// ILI9486: (0xFB) [idx] (reg) [xx] [read]
// HX8357C: (0xFE) [reg] (FF) [read] [read...]
// R61581:  (0xEF) (reg) [F0] [xx] [read] [read...]
// R61581: ?? (reg) [EF] [xx] [read] [read...]
// it would be nice to find if ILI9481 has a sekret register

int read_C0_2(uint8_t sekret)
{
    uint8_t val, d[2], method = 0;
    d[0] = 0x00;
    writeblock(0xB0, d, 1);  //UNLOCK
    d[0] = 0x80;             //covers several bidirectional
    writeblock(0xC6, d, 1);  //enable SDA as input

    d[0] = 0x90 | 2;         //SPI_READ_EN is b4 or b7
    writeblock(sekret, d, 1);       // CS sequence
    val = readwrite8(0xC0, 8, 0); // CS sequence
    if (val != 0xFF) {
        Serial.print("SPIREAD = ");
        printhex(sekret);
        Serial.print(" reg(0xC0.2) = ");
        printhex(val);
        Serial.println("");
    }

    d[0] = 0xC0;         //e.g. (0xEF) [reg]
    writeblock(sekret, d, 1);       // CS sequence
    digitalWrite(TFT_SS, LOW);
    write9(0xFF, 0);
    val = read8(8, 0);
    val = read8(8, 0);
    val = read8(8, 0);
    digitalWrite(TFT_SS, HIGH);
    val = readwrite8(0xC0, 8, 0); // CS sequence
    if (val != 0xFF) {
        Serial.print("SETREADINDEX = ");
        printhex(sekret);
        Serial.print(" reg(0xC0.2) = ");
        printhex(val);
        Serial.println("");
    }

    digitalWrite(TFT_SS, LOW);
    write9(0xC0, 0);
    write9(sekret, 0);
    val = read8(8, 0);
    val = read8(8, 0);
    val = read8(8, 0);
    digitalWrite(TFT_SS, HIGH);
    if (val != 0xFF) {
        Serial.print("READMODE = ");
        printhex(sekret);
        Serial.print(" reg(0xC0.2) = ");
        printhex(val);
        Serial.println("");
    }
    return val != 0xFF;
}

int find_sekret(void)
{
    int reg;
    for (reg = 0x40; reg <= 0x44; reg++) read_C0_2(reg);
    for (reg = 0x46; reg <= 0xA0; reg++) read_C0_2(reg);
    for (reg = 0xA2; reg <= 0xAF; reg++) read_C0_2(reg);
    read_C0_2(0xB1);
    read_C0_2(0xB2);
    for (reg = 0xB5; reg <= 0xBE; reg++) read_C0_2(reg);
    read_C0_2(0xC7);
    for (reg = 0xC9; reg <= 0xCF; reg++) read_C0_2(reg);
    for (reg = 0xd5; reg <= 0xDF; reg++) read_C0_2(reg);
    for (reg = 0xE4; reg <= 0xFF; reg++) read_C0_2(reg);
}

void readReg(uint16_t reg, uint8_t n, const char *msg)
{
    uint8_t val, d[4], dummy;
    dummy = reg == 4 || reg == 9 || reg == 0xBF || reg == 0xA1;
    writeblock(0x01, NULL, 0);   //software reset first
    delay(100);
    if (interface & IS_9481) {
        d[0] = 0x00;
        writeblock(0xB0, d, 1);  //UNLOCK
        d[0] = 0x80;             //covers several bidirectional
        writeblock(0xC6, d, 1);  //enable SDA as input
    }
    Serial.print(" reg(0x");
    printhex(reg);
    Serial.print(")");
    d[0] = (interface & IS_9486) ? 0x80 : 0x10;
    uint8_t sekret = 0;
    if ((interface & IS_9341) && (reg >= 0x10)) sekret = 0xD9;
    //    if ((interface & IS_9481) && (reg >= 0x10)) sekret = 0xEB;
    if ((interface & IS_9486) && (reg >= 0x10)) sekret = 0xFB;
    if (sekret == 0) {
        digitalWrite(TFT_SS, LOW);

        // HX8357C: (0xFE) [reg] (FF) [read] [read...]
        // R61581:  (0xEF) (reg) [F0] [xx] [read] [read...]
        // R61581: ?? (reg) [EF] [xx] [read] [read...]
#define TRICK 6
        if ((interface & IS_9481) && (reg >= 0x10)) {
#if TRICK == 1
            write9(0xFE, 0);
            write9(reg, 1);
            write9(0xFF, 0);
#elif TRICK == 2
            write9(0xFE, 0);
            write9(reg, 1);
            write9(0xF0, 0);
#elif TRICK == 3
            write9(reg, 0);
            write9(0xFB, 0);
#elif TRICK == 4
            write9(reg, 0);
            write9(0xF0, 0);
#elif TRICK == 5
            write9(reg, 0);
            write9(0xEF, 0);
#elif TRICK == 6
            write9(reg, 0);
            write9(0xEB, 0);
#else
            write9(reg, 0);
#endif
        } else

            write9(reg, 0);
    }
    while (n-- > 0) {
        if (sekret ) {
            writeblock(sekret, d, 1);       // CS sequence
            val = readwrite8(reg, 8, 0); // CS sequence
            d[0]++;
        }
        else val = read8(8, dummy);
        dummy = 0;
        Serial.print(" ");
        printhex(val);
    }
    digitalWrite(TFT_SS, HIGH);
    Serial.print("\t");
    Serial.println(msg);
}

void loop() {
    // put your main code here, to run repeatedly:
    yield();
}

#if defined(USE_SPI)
// use SPI mode #0
uint8_t spi(uint8_t c)
{
    SPDR = c;
    while ((SPSR & 0x80) == 0) ;
    return SPDR;
}

uint32_t readwrite8(uint8_t cmd, uint8_t bits, uint8_t dummy)
{
    uint32_t ret = 0;
    uint8_t val = cmd;
    int cnt = 8;
    digitalWrite(TFT_SS, LOW);
    SPSR = (0 << SPI2X);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); //1MHz
    digitalWrite(TFT_DC, LOW);
    pinMode(TFT_MOSI, OUTPUT);
    spi(cmd);
    if (bits) {
        digitalWrite(TFT_DC, HIGH);
        pinMode(TFT_MOSI, INPUT);
        while (bits) {
            ret <<= 8;
            ret |= spi(0x00);
            bits -= 8;
        }
        ret >>= dummy;
    }
    digitalWrite(TFT_SS, HIGH);
    return ret;
}
#endif

