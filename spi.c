#include "spi.h"

uint8_t CS;
uint8_t MISO;
uint8_t MOSI;
uint8_t CLOCK;

void spi_init(){
    CS = PIN(9,0);
    MISO = PIN(10,0);
    MOSI = PIN(8,0);
    CLOCK = PIN(7,0);
    pinMode(CS, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);
}


void spi_habilita(){
    pinMode(CS, OUTPUT);
    digitalWrite(CS, LOW);
} // coloca o CS (enable / disable do componente) para 0, isto é, liga o dispositivo


void spi_desabilita(){
    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);
} // coloca o CS (enable / disable do componente) para 1, isto é, desliga o dispositivo


uint8_t spi_write(uint8_t byte){
    pinMode(MOSI, OUTPUT);
    uint8_t valor = 0;
    uint8_t bit = 0;
    for(uint8_t x = 0; x < 8; x++){
        bit = (byte >> (7 - x))&1;
        digitalWrite(MOSI, bit);
        digitalWrite(CLOCK, HIGH);
        delay_ms(50);
        valor = (valor << 1)|digitalRead(MISO);
        digitalWrite(CLOCK, LOW);
        delay_ms(50); 
    }
    return valor;
} 