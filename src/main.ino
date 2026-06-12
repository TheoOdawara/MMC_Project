#include <Wire.h>

int estado = 0;
int passo_atual = 0;
int temperatura_adc = 0;

#define ADS1115_ADDR 0x48
#define ADS1115_REG_CONVERSION 0x00
#define ADS1115_REG_CONFIG 0x01

// AIN0 single-ended, single-shot, gain +-2.048V, 128 SPS, comparator off.
#define ADS1115_CONFIG_HI 0xC5
#define ADS1115_CONFIG_LO 0x83

// LM35 = 10 mV/C. 6016 counts on the ADS1115 (16-bit, +-2.048V) ~= 0.376 V ~= 37.6 C.
#define LIMITE 6016

void motor_passo(int p) {
  PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3));

  if (p == 0) {
    PORTB |= (1 << PB1);
    PORTB |= (1 << PB2);
  }
  else if (p == 1) {
    PORTB |= (1 << PB2);
  }
  else if (p == 2) {
    PORTB |= (1 << PB0);
    PORTB |= (1 << PB2);
  }
  else if (p == 3) {
    PORTB |= (1 << PB0);
  }
  else if (p == 4) {
    PORTB |= (1 << PB0);
    PORTB |= (1 << PB3);
  }
  else if (p == 5) {
    PORTB |= (1 << PB3);
  }
  else if (p == 6) {
    PORTB |= (1 << PB1);
    PORTB |= (1 << PB3);
  }
  else if (p == 7) {
    PORTB |= (1 << PB1);
  }
}

void motor_girar(int passos) {
  int i;

  if (passos > 0) {
    for (i = 0; i < passos; i++) {
      passo_atual = passo_atual + 1;
      if (passo_atual > 7) {
        passo_atual = 0;
      }
      motor_passo(passo_atual);
      delayMicroseconds(5000);
    }
  }
  else {
    for (i = 0; i < (-passos); i++) {
      passo_atual = passo_atual - 1;
      if (passo_atual < 0) {
        passo_atual = 7;
      }
      motor_passo(passo_atual);
      delayMicroseconds(5000);
    }
  }
}

void ler_adc() {
  Wire.beginTransmission(ADS1115_ADDR);
  Wire.write(ADS1115_REG_CONFIG);
  Wire.write(ADS1115_CONFIG_HI);
  Wire.write(ADS1115_CONFIG_LO);
  Wire.endTransmission();

  delay(8);

  Wire.beginTransmission(ADS1115_ADDR);
  Wire.write(ADS1115_REG_CONVERSION);
  Wire.endTransmission();

  Wire.requestFrom(ADS1115_ADDR, 2);
  uint8_t hi = Wire.read();
  uint8_t lo = Wire.read();

  uint16_t raw = ((uint16_t)hi << 8) | (uint16_t)lo;
  temperatura_adc = (int16_t)raw;
}

void setup() {
  DDRD |= (1 << PD5);
  DDRD |= (1 << PD6);
  DDRD |= (1 << PD7);

  DDRB |= (1 << PB0);
  DDRB |= (1 << PB1);
  DDRB |= (1 << PB2);
  DDRB |= (1 << PB3);

  Wire.begin();

  motor_passo(0);

  PORTD |= (1 << PD5);
}

void loop() {
  if (estado == 0) {
    PORTD &= ~(1 << PD5);
    estado = 1;
  }
  else if (estado == 1) {
    ler_adc();

    if (temperatura_adc < LIMITE) {
      estado = 2;
    }
    else {
      estado = 3;
    }
  }
  else if (estado == 2) {
    PORTD |= (1 << PD6);

    motor_girar(200);

    delay(3000);

    motor_girar(-200);

    PORTD &= ~((1 << PD5) | (1 << PD6) | (1 << PD7));
    PORTD |= (1 << PD5);
    estado = 0;
  }
  else if (estado == 3) {
    PORTD |= (1 << PD7);

    delay(3000);

    PORTD &= ~((1 << PD5) | (1 << PD6) | (1 << PD7));
    PORTD |= (1 << PD5);
    estado = 0;
  }
}
