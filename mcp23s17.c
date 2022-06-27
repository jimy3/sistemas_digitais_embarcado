#include "mcp23s17.h"

uint8_t _reg[22];   // Local mirrors of the 22 internal registers of the MCP23S17 chip
uint8_t _addr = 0; // é o endereço do componenet, que pé definido como 0 pela gente arbitrairamente

void mcp23S17_init(){

    _reg[MCP_IODIRA] = 255;
    _reg[MCP_IODIRB] = 255;
    _reg[MCP_IPOLA] = 0;
    _reg[MCP_IPOLB] = 0;
    _reg[MCP_GPINTENA] = 0;
    _reg[MCP_GPINTENB] = 0;
    _reg[MCP_DEFVALA] = 0;
    _reg[MCP_DEFVALB] = 0;
    _reg[MCP_INTCONA] = 0;
    _reg[MCP_INTCONB] = 0;
    _reg[MCP_IOCONA] = 24;
    _reg[MCP_IOCONB] = 24;
    _reg[MCP_GPPUA] = 0;
    _reg[MCP_GPPUB] = 0;
    _reg[MCP_INTFA] = 0;
    _reg[MCP_INTFB] = 0;
    _reg[MCP_INTCAPA] = 0;
    _reg[MCP_INTCAPB] = 0;
    _reg[MCP_GPIOA] = 0;
    _reg[MCP_GPIOB] = 0;
    _reg[MCP_OLATA] = 0;
    _reg[MCP_OLATB] = 0;

    spi_init();
    spi_desabilita();
    spi_habilita();

    uint8_t cmd = 0b01000000; // decimal = 64

    spi_write(cmd);
    spi_write(8); // porque o MCP_IOCONA é o oitavo registrador do MCP
    spi_write(0x18);

    spi_desabilita();

    writeAll();
}

void writeAll() {
    uint8_t cmd = 0b01000000 | ((_addr & 0b111) << 1); // Estudar essa operação
    spi_habilita();
    spi_write(cmd);
    spi_write(0);

    for (uint8_t i = 0; i < 22; i++) {
        spi_write(_reg[i]);
    }
    spi_desabilita();
}

// Escreve o valor contido no nosso array _reg dentro do Chip do MCP.
// Observação: o MCP possui 22 registradores configuráveis;
void escreve_no_registrador(uint8_t addr){
    uint8_t cmd = 0b01000000 | ((_addr & 0b111) << 1); // Estudar essa operação

    spi_habilita();
    spi_write(cmd);
    spi_write(addr);
    spi_write(_reg[addr]);
    spi_desabilita();
}

void le_registrador(uint8_t addr) {
    uint8_t cmd = 0b01000001 | ((_addr & 0b111) << 1); // Estudar essa operação
    spi_habilita();
    spi_write(cmd);
    spi_write(addr);
    _reg[addr] = spi_write(0xFF); // 1111 1111
    spi_desabilita();
}

void mcp23S17_configura_pino(uint8_t pino, uint8_t modo){
    uint8_t dirReg = MCP_IODIRA; // configura a direção de cada pino do port A
    uint8_t puReg = MCP_GPPUA; // configura registrador de pull up de cada pino do port A

    if (pino >= 8) {
        pino -= 8;
        dirReg = MCP_IODIRB;
        puReg = MCP_GPPUB;
    }

    switch (modo) {
        case OUTPUT:
            _reg[dirReg] &= ~(1<<pino); // Estudar essa operação
            escreve_no_registrador(dirReg);
            break;
        case INPUT:
             _reg[dirReg] |= (1<<pino);
            escreve_no_registrador(dirReg);
            _reg[puReg] &= ~(1<<pino);
            escreve_no_registrador(puReg);
            break;
    }
}

void mcp23S17_escreve_pino(uint8_t pino, uint8_t valor){
    // deve começar escrevendo um byte de opcode
    // em seguida, escrever os dados que você quer escrever no MCP (Register Adress)
    // operation code: 0 1 0 0 A2 A1 A0 RW
    // RW -> 0 = write ||| RW -> 1 = read
    // register a7 a6 a5 a4 ... a0
    uint8_t dirReg = MCP_IODIRA;
    uint8_t puReg = MCP_GPPUA;
    uint8_t latReg = MCP_OLATA;
    if (pino >= 8) {
        pino -= 8;
        dirReg = MCP_IODIRB;
        puReg = MCP_GPPUB;
        latReg = MCP_OLATB;
    }

    uint8_t modo = (_reg[dirReg] & (1<<pino)) == 0 ? OUTPUT : INPUT; // Estudar essa operação

    switch (modo) {
        case OUTPUT:
            if (valor == 0) {
                _reg[latReg] &= ~(1<<pino);
            } else {
                _reg[latReg] |= (1<<pino);
            }
            escreve_no_registrador(latReg);
            break;

        case INPUT:
            if (valor == 0) {
                _reg[puReg] &= ~(1<<pino);
            } else {
                _reg[puReg] |= (1<<pino);
            }
            escreve_no_registrador(puReg);
            break;
    }
}


uint8_t mcp23S17_le_pino(uint8_t pino){
    uint8_t dirReg = MCP_IODIRA;
    uint8_t portReg = MCP_GPIOA;
    uint8_t latReg = MCP_OLATA;
    if (pino >= 8) {
        pino -= 8;
        dirReg = MCP_IODIRB;
        portReg = MCP_GPIOB;
        latReg = MCP_OLATB;
    }

    uint8_t modo = (_reg[dirReg] & (1<<pino)) == 0 ? OUTPUT : INPUT; // Estudar essa operação

    switch (modo) {
        case OUTPUT:
            return _reg[latReg] & (1<<pino) ? HIGH : LOW;
        case INPUT:
            le_registrador(portReg);
            return _reg[portReg] & (1<<pino) ? HIGH : LOW;
    }
    return 0;

}
