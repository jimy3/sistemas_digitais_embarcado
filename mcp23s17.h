#ifndef __MCP23S17__
#define __MCP23S17__

#include <stdint.h>
#include<stdio.h>
#include "spi.h"

#define MCP_IODIRA 0
#define MCP_IODIRB 1
#define MCP_IPOLA 2
#define MCP_IPOLB 3
#define MCP_GPINTENA 4
#define MCP_GPINTENB 5
#define MCP_DEFVALA 6
#define MCP_DEFVALB 7
#define MCP_INTCONA 8
#define MCP_INTCONB 9
#define MCP_IOCONA 10
#define MCP_IOCONB 11
#define MCP_GPPUA 12
#define MCP_GPPUB 13
#define MCP_INTFA 14
#define MCP_INTFB 15
#define MCP_INTCAPA 16
#define MCP_INTCAPB 17
#define MCP_GPIOA 18
#define MCP_GPIOB 19
#define MCP_OLATA 20
#define MCP_OLATB 21

void mcp23S17_init();
void writeAll();
void le_registrador(uint8_t addr);

void mcp23S17_configura_pino(uint8_t pino, uint8_t modo);
void mcp23S17_escreve_pino(uint8_t pino, uint8_t valor);
uint8_t mcp23S17_le_pino(uint8_t pino);
void escreve_no_registrador(uint8_t addr);

#endif
