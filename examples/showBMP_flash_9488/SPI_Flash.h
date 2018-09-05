extern void read_spi_mem(uint32_t loc, uint8_t *buf, int16_t n, int select);
#define FLASHBUFSIZ 256

class SPI_Flash {
    public:
        SPI_Flash(void) { }

        void open(int32_t ads = 0) {
            _ads = ads;
            _pos = 0;
            _left = 0;
        }
        int16_t read(void *vblock, int sz) {
            uint8_t *block = (uint8_t*)vblock;
            while (sz > 0) {
                if (_left <= 0) {
                    read_spi_mem(_pos + _ads, _buf, FLASHBUFSIZ, FLASH_CS);
                    _bufptr = _buf;
                    _left = FLASHBUFSIZ;
                }
                int n = (sz < _left) ? sz :  _left;
                memcpy(block, _bufptr, n);
                _left -= n;
                _bufptr += n;
                _pos += n;
                block += n;
                sz -= n;
            }
        }
        int32_t seek(int32_t ofs) {
            int32_t where = ofs - _pos;
            if (where < 0 || where > _left) _left = 0;
            else {
                _left -= where;
                _bufptr += where;
            }
            return _pos = ofs;
        }
        int32_t position(void) {
            return _pos;
        }
        void close(void) { }
    private:
        uint8_t _buf[FLASHBUFSIZ], *_bufptr;
        int16_t _left;
        int32_t _ads, _pos;
};


