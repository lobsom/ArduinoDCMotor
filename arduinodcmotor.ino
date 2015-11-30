#define LONG_PRESS 20
#define SHORT_PRESS 1
#define RAMPA 10

//componentes
int botao = 2;
int potenciometro = A0;
int cw = 6;
int ccw = 5;

//estado que define o que fazer após um long_press
int SITUACAO_MOTOR = 0; // 1 = LIGADO | 0 = DESLIGADO
int SENTIDO = 0; // 0 - HORARIO | 1 = ANTI HORARIO
int VELOCIDADE_ATUAL = 66; // variavel global para trabalhar com uma simulação entre velocidade atual e ideal

void setup() {
  Serial.begin(9600);
  pinMode(botao, INPUT);
  pinMode(potenciometro, INPUT);
}

// le o potenciometro e retorna valor mapeado em até 255
int ler_potenciometro() {
  return map(analogRead(potenciometro), 0, 1023, 0, 255);
}

void acelerar() {
  if(SITUACAO_MOTOR == 1) {
    int velocidade_final = ler_potenciometro();
    int aceleracao = velocidade_final - VELOCIDADE_ATUAL;
    int gradacao = aceleracao / RAMPA;

  for (int i = 0; i < gradacao; i++) {
      if (SENTIDO == 0) {
        analogWrite(cw, VELOCIDADE_ATUAL + RAMPA);
        analogWrite(ccw, LOW);
      }
      else {
        analogWrite(ccw, VELOCIDADE_ATUAL + RAMPA);
        analogWrite(cw, LOW);
      }
    VELOCIDADE_ATUAL = VELOCIDADE_ATUAL + RAMPA;
    delay(RAMPA);
   }
 }
}

void desacelerar() {
  if (SITUACAO_MOTOR == 1){
    int velocidade_final = ler_potenciometro();
    int desaceleracao = velocidade_final;
    int gradacao = desaceleracao / RAMPA;
  for (int i = 0; i < gradacao; i++) {
      if (SENTIDO == 0) {
        analogWrite(cw, VELOCIDADE_ATUAL - RAMPA);
        analogWrite(ccw, LOW);
      }
      else {
        analogWrite(ccw, VELOCIDADE_ATUAL - RAMPA);
        analogWrite(cw, LOW);
      }
    VELOCIDADE_ATUAL = VELOCIDADE_ATUAL - RAMPA;
    delay(RAMPA);
   }
 }
}

void frear() {
  digitalWrite(cw, HIGH);
  digitalWrite(ccw, HIGH);
}

void desligar_motor() {
  frear();
  digitalWrite(cw, LOW);
  digitalWrite(ccw, LOW);
  SITUACAO_MOTOR = 0;
}

// liga o mortor
void ligar_motor() {
  SITUACAO_MOTOR = 1;
  acelerar();
}

void variar_velocidade() {
    if ((ler_potenciometro() - VELOCIDADE_ATUAL) > 0){
      acelerar();
    }
    else {
      desacelerar();
    }
}

// inverter rotação
void inverter_motor() {

  frear();
  delay(10);

  if (SENTIDO == 0) {
    SENTIDO = 1;
    acelerar();
  } else {
    SENTIDO = 0;
    acelerar();
  }
}

// decide [e chama] a função relacionada ao tempo pressionado
void ligar_ou_desligar(int tempo_pressionado){
    if (tempo_pressionado >= LONG_PRESS & SITUACAO_MOTOR == 0) {
      Serial.println("LIGAR O MOTOR");
      ligar_motor();
    }
    else {
      Serial.println("FREAR");
      desligar_motor();
    }
}

void loop() {

  int tempo_pressionado = 0;
  while(digitalRead(botao) == HIGH) {
    delay(100);
    tempo_pressionado++;
  }

  if (tempo_pressionado >= LONG_PRESS) {
    Serial.println("AVALIAR O QUE FAZER");
    ligar_ou_desligar(tempo_pressionado);
  }
  else if (tempo_pressionado > 0 & tempo_pressionado < LONG_PRESS) {
    Serial.println("INVERTER O MOTOR");
    inverter_motor();
  }
  else if (tempo_pressionado == 0 & SITUACAO_MOTOR == 1) {
    variar_velocidade();  // acelerar, desacelerar ou manter a velocidade atual
  }
  delay(10);
}
