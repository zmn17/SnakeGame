#include "Characters.h"
#include <Arduino.h>
#include <cstdint>

// Define the snake character
uint8_t body[8] = {0b00000, 0b11111, 0b11111, 0b11111,
                   0b11111, 0b11111, 0b11111, 0b00000};
                   
uint8_t r_head[8] = {0b00000, 0b11000, 0b11100, 0b11111,
                     0b11100, 0b11000, 0b00000};

uint8_t l_head[8] = {0b00000, 0b00011, 0b00111, 0b11111,
                     0b00111, 0b00011, 0b00000};

uint8_t u_head[8] = {0b00100, 0b01110, 0b11111, 0b11111, 0b00000};

uint8_t d_head[8] = {0b00000, 0b11111, 0b11111, 0b01110, 0b00100};

// other characters: foods
uint8_t apple[8] = {
  0b00100,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

uint8_t banana[8] = {
  0b00010,
  0b00010,
  0b00010,
  0b00110,
  0b01100,
  0b11000,
  0b11000,
  0b00000
};

uint8_t bread[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

