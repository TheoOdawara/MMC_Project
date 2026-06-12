int estado = 0;
int passo_atual = 0;
int temperatura_adc = 0;

#define LIMITE 77

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
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  temperatura_adc = ADC;
}

void setup() {
  DDRD |= (1 << PD5);
  DDRD |= (1 << PD6);
  DDRD |= (1 << PD7);
  DDRD |= (1 << PD3);

  DDRD &= ~(1 << PD2);
  PORTD |= (1 << PD2);

  DDRB |= (1 << PB0);
  DDRB |= (1 << PB1);
  DDRB |= (1 << PB2);
  DDRB |= (1 << PB3);

  ADMUX = (1 << REFS0) | 0;
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  motor_passo(0);

  PORTD |= (1 << PD5);
}

void loop() {
  if (estado == 0) {
    if ((PIND & (1 << PD2)) == 0) {
      delay(50);
      if ((PIND & (1 << PD2)) == 0) {
        PORTD &= ~(1 << PD5);
        estado = 1;
      }
    }
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

    PORTD |= (1 << PD3);
    delay(200);
    PORTD &= ~(1 << PD3);
    delay(50);

    motor_girar(200);

    delay(3000);

    motor_girar(-200);

    PORTD &= ~((1 << PD5) | (1 << PD6) | (1 << PD7));
    PORTD |= (1 << PD5);
    estado = 0;
  }
  else if (estado == 3) {
    PORTD |= (1 << PD7);

    int i;
    for (i = 0; i < 3; i++) {
      PORTD |= (1 << PD3);
      delay(150);
      PORTD &= ~(1 << PD3);
      delay(150);
    }

    delay(3000);

    PORTD &= ~((1 << PD5) | (1 << PD6) | (1 << PD7));
    PORTD |= (1 << PD5);
    estado = 0;
  }
}
