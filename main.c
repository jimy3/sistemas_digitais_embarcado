#include <stdio.h>
#include <stdint.h>
#include "mcp23s17.h"

MCP_P_15 = PIN(15,0);
MCP_P_0 = PIN(0,0);

int main() {
    mcp23S17_init();
    mcp23S17_configura_pino(MCP_P_15, INPUT);
    mcp23S17_configura_pino(MCP_P_0 , OUTPUT);

    while(1) {
        mcp23S17_escreve_pino(MCP_P_0, HIGH);
        delay_ms(1000);
        mcp23S17_escreve_pino(MCP_P_0, LOW);
        delay_ms(1000);

        if (mcp23S17_le_pino(MCP_P_15)==HIGH) {
            printf("Botao em VCC\n");
        }
            else printf("Botao em GND\n");
        }
}
