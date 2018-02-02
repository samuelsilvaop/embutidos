#include <L298N.h>
#include <SoftwareSerial.h>
#include "PS2Mouse.h"

#define velA 9
#define velB 10
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

L298N motor1(velA, IN1, IN2);
L298N motor2(velB, IN3, IN4);
PS2Mouse mouse(3, 2);
SoftwareSerial bluetooth(12, 11);

char total[8];
char comandox[5];
char comandoy[5];
int cmdx, cmdy, i, x, y, teste;
uint8_t stat;
long tInicio, tFim, deslocax = 0, deslocay = 0;

void leMouse() {
  mouse.getPosition(stat, x, y);
//  x = map(x, -127, 127, -15, 15);
//  y = map(y, -127, 127, -15, 15);
  deslocax += x;
  deslocay += y;
//  Serial.print(stat, BIN);
//  Serial.print("\tx=");
//  Serial.print(x, DEC);
//  Serial.print("\ty=");
//  Serial.print(y, DEC);
//  Serial.print("\tTempo:");
//  Serial.println(millis());
}

void anda() {
  if (cmdy <= 278)
    esquerda();
  else
    direita();
}

int frente(int teste) {
  leMouse();
  tInicio = millis();
  tFim = 0;
  motor1.setSpeed(95);
  motor2.setSpeed(90);
  motor1.forward();
  motor2.forward();
  while (teste > tFim - tInicio) {
    tFim = millis();
    leMouse();
  }
}
void freia() {  //Freia os motores
  motor1.stop();
  motor2.stop();
  deslocax = deslocax*0.15/127;
  deslocay = deslocay*0.15/127;
  Serial.print("\tDeslocamento em x:");
  Serial.print(deslocax);
  Serial.print("\tDeslocamento em y:");
  Serial.println(deslocay);
  while (1) {}
}

void esquerda() {
  leMouse();
  frente(cmdx);  //Gira o Motor A e B no sentido horario
  tInicio = millis();
  tFim = 0;
  motor1.setSpeed(95);
  motor2.setSpeed(90);
  motor1.forward();
  motor2.backward();
  while (400 > tFim - tInicio) {
    leMouse();
    tFim = millis();
  }
  frente(cmdy); //Gira o Motor A e B no sentido horario
  freia();
}

void direita() {
  leMouse();  //Chama a função de leitura dos dados do mouse
  frente(cmdx); //Chama a função que movimenta o veículo
  tInicio = millis();
  tFim = 0;
  motor1.setSpeed(95);  //Define a velocidade do motor1
  motor2.setSpeed(90);  //Define a velocidade do motor2
  motor1.backward();    //Define o sentido de giro do motor1
  motor2.forward();     //Define o sentido de giro do motor2
  while (400 > tFim - tInicio) {
    leMouse();
    tFim = millis();
  }
  frente(cmdy);  //Gira o Motor A e B no sentido horario
  freia();
}

void setup() {
  bluetooth.begin(9600);//Inicia a comunicação por Bluetooth
  Serial.begin(9600);
  while(!Serial);
  Serial.print("Setup...");

  mouse.begin();
  Serial.println("complete!\n");
}

void loop() {
  while (bluetooth.available()) {
    for (i = 0; i <= 8; i++) {
      total[i] = bluetooth.read();
      delay(50);
    }

    for (i = 0; i < 4; i++) {
      comandox[i] = total[i];
      comandoy[i] = total[i + 4];
    }

    cmdx = atoi(comandox);
    cmdy = atoi(comandoy);

    anda();
  }
}

