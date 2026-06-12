# Esquema de Montagem — Controle de Acesso Térmico

## Diagrama de conexões

```
┌─────────────────────────────────────────────────────────────┐
│                         ARDUINO UNO                         │
├───────────────┬─────────────────────────────────────────────┤
│  5V ──────────┼──────→ ADS1115 VDD                          │
│               │       → ULN2003 (+) (pino + do driver)     │
│               │       → LM35 VCC (pino 1)                  │
│               │       → Buzzer (+) (se for piezo ativo)    │
├───────────────┼─────────────────────────────────────────────┤
│  GND ─────────┼──────→ ADS1115 GND                         │
│               │       → ULN2003 (-) (pino - do driver)     │
│               │       → LM35 GND (pino 3)                  │
│               │       → LEDs (catodo / perna curta)         │
│               │       → Botão (outro lado)                 │
│               │       → Buzzer (-)                         │
├───────────────┼─────────────────────────────────────────────┤
│  A4 (SDA) ────┼──────→ ADS1115 SDA                         │
│  A5 (SCL) ────┼──────→ ADS1115 SCL                         │
├───────────────┼─────────────────────────────────────────────┤
│  A0           ┃     ←── LM35 OUT (pino 2) → ADS1115 A0    │
├───────────────┼─────────────────────────────────────────────┤
│   8 (PB0) ────┼──────→ ULN2003 IN1                         │
│   9 (PB1) ────┼──────→ ULN2003 IN2                         │
│  10 (PB2) ────┼──────→ ULN2003 IN3                         │
│  11 (PB3) ────┼──────→ ULN2003 IN4                         │
├───────────────┼─────────────────────────────────────────────┤
│   5 (PD5) ────┼──────→ [R 220Ω] ───→ LED Azul (anodo +)   │
│   6 (PD6) ────┼──────→ [R 220Ω] ───→ LED Verde (anodo +)  │
│   7 (PD7) ────┼──────→ [R 220Ω] ───→ LED Vermelho (anodo +)│
├───────────────┼─────────────────────────────────────────────┤
│   2 (PD2) ────┼──────→ Botão ───→ GND                      │
│               │        (pull-up interno ativado no código)  │
├───────────────┼─────────────────────────────────────────────┤
│   3 (PD3) ────┼──────→ Buzzer (+)                           │
└───────────────┴─────────────────────────────────────────────┘
```

## Lista de materiais

| Componente | Quantidade |
|---|---|
| Arduino Uno | 1 |
| LM35 | 1 |
| ADS1115 | 1 |
| Motor 28BYJ-48 + Driver ULN2003 | 1 |
| LED azul | 1 |
| LED verde | 1 |
| LED vermelho | 1 |
| Resistor 220Ω | 3 |
| Resistor 4,7kΩ (pull-up I2C) | 2 (se o módulo ADS1115 não tiver) |
| Botão push-button (NA) | 1 |
| Buzzer (ativo ou piezo) | 1 |
| Protoboard | 1 |
| Jumpers macho-macho | ~20 |

## Passo a passo

### 1. Protoboard — trilhos de energia
- Conecte o **5V do Arduino** ao trilho positivo (+) da protoboard
- Conecte o **GND do Arduino** ao trilho negativo (-) da protoboard

### 2. LM35 + ADS1115

```
LM35 (visto de frente, parte plana virada para você):
  ┌───┐
  │ 1 │ → VCC (5V)
  │ 2 │ → OUT → ADS1115 A0
  │ 3 │ → GND
  └───┘

ADS1115:
  VDD  → 5V
  GND  → GND
  SCL  → A5 (Arduino)
  SDA  → A4 (Arduino)
  ADDR → GND (endereço I2C padrão 0x48)
  A0   → LM35 OUT (pino 2)
```

Se o módulo ADS1115 **não** tiver resistores de pull-up embutidos em SDA/SCL, adicione resistores de **4,7kΩ** entre SDA → 5V e SCL → 5V.

### 3. Motor 28BYJ-48 + Driver ULN2003

O driver ULN2003 já vem acoplado ao motor (conector de 5 pinos). O lado do driver tem 6 pinos:

```
ULN2003 Driver:
  ┌─────────────────────────────────────┐
  │ IN1  IN2  IN3  IN4  [-]  [+]        │
  │  │    │    │    │    │    │          │
  │  8    9   10   11  GND  5V          │
  │  └──── Conecte ao Arduino ──────────┘
  │
  │  [Conector para o motor 28BYJ-48]
  └─────────────────────────────────────┘
```

- Encaixe o conector do motor no driver (só entra de um jeito)
- IN1 → pino 8 do Arduino
- IN2 → pino 9 do Arduino
- IN3 → pino 10 do Arduino
- IN4 → pino 11 do Arduino
- `+` do driver → 5V
- `-` do driver → GND

### 4. LEDs

Cada LED segue o mesmo padrão:

```
Pino Arduino → Resistor 220Ω → Anodo (+ / perna longa) do LED
                                Catodo (- / perna curta) → GND
```

| LED | Pino Arduino | Função |
|-----|-------------|--------|
| Azul | 5 | Sistema pronto / esperando |
| Verde | 6 | Acesso liberado |
| Vermelho | 7 | Acesso negado |

### 5. Botão

O código ativa o pull-up interno do ATmega328P, então **não precisa de resistor externo**.

```
Arduino pino 2 ──── Botão ──── GND
```

Quando pressionado, o pino 2 é conectado ao GND (LOW).

### 6. Buzzer

```
Arduino pino 3 ──── Buzzer (+) ──── Buzzer (-) ──── GND
```

- Buzzer **ativo**: gera som sozinho com nível HIGH
- Buzzer **passivo**: o delay de 150–200ms no código produz uma frequência audível
- A perna mais longa geralmente é o positivo

## Checklist antes de ligar

1. ✅ Todos os **GNDs** conectados ao mesmo trilho negativo
2. ✅ **LEDs** com resistor de 220Ω em série (nunca ligar LED sem resistor)
3. ✅ **ADS1115** — VDD → 5V, GND → GND, ADDR → GND
4. ✅ **ULN2003** — `+` → 5V, `-` → GND
5. ✅ **LM35** — pino 1 (VCC, 5V), pino 2 (OUT → ADS1115 A0), pino 3 (GND)
6. ✅ **Botão** — conectado ao pino 2 e ao GND (sem resistor externo)
7. ✅ **Pinos I2C** — SDA em A4, SCL em A5
8. ✅ **Resistores de pull-up I2C** — presentes (no módulo ou externos de 4,7kΩ)
9. ✅ Protoboard bem pressionada, jumpers firmes

## Pinagem (resumo)

| Componente | Pino Arduino |
|---|---|
| ADS1115 SDA | A4 |
| ADS1115 SCL | A5 |
| LM35 OUT | ADS1115 A0 |
| Motor IN1 | 8 |
| Motor IN2 | 9 |
| Motor IN3 | 10 |
| Motor IN4 | 11 |
| LED Verde | 6 |
| LED Vermelho | 7 |
| LED Azul | 5 |
| Botão (INPUT_PULLUP) | 2 |
| Buzzer | 3 |
