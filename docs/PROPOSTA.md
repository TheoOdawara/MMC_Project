# Proposta de Projeto — Controle de Acesso Térmico

## Cenário
Catraca eletrônica que libera a passagem apenas se a temperatura da pessoa estiver dentro do limite seguro (< 37,5 °C). O usuário solicita o acesso pressionando um botão, o sistema mede a temperatura via LM35 + ADS1115, e decide se libera (motor de passo gira a catraca) ou nega (LED vermelho + buzzer de alarme).

## Componentes

| Componente | Função |
|---|---|
| Arduino Uno | Controle do sistema |
| LM35 | Sensor de temperatura |
| ADS1115 | ADC de 16 bits para leitura precisa do LM35 |
| Motor de passo 28BYJ-48 + driver ULN2003 | Gira a catraca (libera/bloqueia) |
| LED verde | Indica acesso liberado |
| LED vermelho | Indica acesso negado |
| LED azul | Indica sistema pronto |
| Botão | Usuário solicita o acesso |
| Buzzer | Feedback sonoro (liberado = 1 bip, negado = alarme intermitente) |
| Protoboard + jumpers | Montagem |

## Funcionamento (passo a passo)

1. **Sistema pronto** — LED azul aceso, motor parado na posição fechada.
2. Usuário **pressiona o botão** para solicitar acesso.
3. O sistema lê a temperatura ambiente via **LM35 + ADS1115**.
4. **Decisão:**
   - **< 37,5 °C → ACESSO LIBERADO**
     - LED verde acende
     - Buzzer dá 1 bip curto
     - Motor de passo gira 180° para abrir a catraca
     - Aguarda 3 segundos
     - Motor retorna à posição fechada
   - **≥ 37,5 °C → ACESSO NEGADO**
     - LED vermelho acende
     - Buzzer dispara alarme intermitente (beep-beep-beep)
     - Motor **não** abre
5. Sistema volta ao estado "pronto" (LED azul).

## Pinagem (Arduino → componentes)

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

## Demonstração ao vivo (na apresentação)

1. **Caso 1 — Acesso liberado:** sem encostar no sensor (temp. ambiente ~25 °C), apertar o botão. LED verde acende, buzzer bipa, motor gira a catraca.
2. **Caso 2 — Acesso negado:** aquecer o LM35 segurando com os dedos por alguns segundos, apertar o botão. LED vermelho acende, buzzer dispara alarme, motor não gira.
3. Todos os componentes obrigatórios (LM35, ADS1115, motor de passo, LEDs) e extras (botão, buzzer) são demonstrados.

## Bibliotecas necessárias (Arduino IDE)

- `Adafruit_ADS1X15` — comunicação com o ADS1115
- `Stepper` ou controle manual do motor 28BYJ-48

## Estrutura do código (proposta)

```
src/
  main.ino              → setup() + loop() + máquina de estados
  temp_sensor.h/.cpp    → leitura do LM35 via ADS1115
  stepper_motor.h/.cpp  → controle do motor de passo
  indicators.h/.cpp     → LEDs + buzzer
```

## Observações

- Toda a montagem será feita em protoboard — sem necessidade de estrutura física.
- O motor 28BYJ-48 já vem com o driver ULN2003, facilitando a ligação.
- O ADS1115 permite leitura de temperatura com precisão de 16 bits, justificando seu uso no projeto.

---

**Integrantes:** (preencher nomes)
