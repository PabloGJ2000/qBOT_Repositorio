/*Código [TEST] qBOT BLUE

   Autor: Pablo García Jaén
   Web: www.qbotproject.com
   Licencia: CC BY-NC-SA
   Repositorio: https://github.com/PabloGarciaJaen/qBOT_Repositorio

*/


//Librerias
#include <Wire.h>
#include "FabricaDigital_MCP23008.h"
#include <Servo.h>

const int MCP23008_I2C_DIR = 0x20; // Dirección I2C del MCP23008
FabricaDigital_MCP23008 pinesMotores(MCP23008_I2C_DIR);

//Declaramos los servos
Servo SERVO_A;
Servo SERVO_B;
Servo SERVO_C;

const int TIEMPO = 15;
const int TIEMPO_SERVOS = 10;

boolean activador_motor;
boolean activador_servo;
unsigned long tiempoAnterior = 0;

int GIRO = 0;

void TEMPORIZADOR() {
  //DEVOLVEMOS EL VALOR OPUESTO (1->0     0->1) DE activador_motor Y activador_servo.
  //ESTO NOS PERMITIRÁ QUE EL SERVO Y EL GEARMOTOR GIRE A UNA VELOCIDAD MAS LENTA.
  if (millis() - tiempoAnterior >= TIEMPO) { //si ha transcurrido el periodo programado
    activador_motor = !activador_motor;
    activador_servo = !activador_servo;
    tiempoAnterior = millis(); //guarda el tiempo actual como referencia
  }
}

void GIRAR_GEARMOTOR(int motor_X_A, int motor_X_B, int potenciometro, int valorMIN, int valorMAX ) {
  const int TIEMPO_GEARMOTOR = 20;
  int bucle = 1;
  while (bucle == 1) {
    int valorPOT;
    valorPOT = analogRead(potenciometro);
    Serial.println(valorPOT);

    //ESTA FUNCION NOS DEVOLVERÁ EL OPUESTO DE LA VARIABLE activador_motor CADA CIERTO TIEMPO.
    //GRACIAS A ESTO, LOS MOTORES SOLO SE ACTIVARÁN UNA VEZ DE CADA X MILISEGUNDO, REDUCIENDO SU VELOCIDAD.
    TEMPORIZADOR();

    if (activador_motor == 1) {  //ENTRAMOS SI NOS LO PERMITE LA FUNCION TEMPORIZADOR();
      if (valorPOT < valorMIN) {      //SI EL VALOR QUE LEEMOS DEL POTENCIOMETRO ES MENOR QUE EL MINIMO GIRAMOS HACIA UN LADO.
        digitalWrite(motor_X_A, LOW);
        digitalWrite(motor_X_B, HIGH);
      }
      if (valorPOT > valorMAX) { //SI EL VALOR QUE LEEMOS DEL POTENCIOMETRO ES MAYOR QUE EL MAXIMO GIRAMOS HACIA EL OTRO LADO.
        digitalWrite(motor_X_A, HIGH);
        digitalWrite(motor_X_B, LOW);
      }
      if ((valorPOT >= valorMIN) && (valorPOT <= valorMAX)) {  //SI EL VALOR QUE LEEMOS DEL POTENCIOMETRO ESTÁ COMPRENDIDO ENTRE EL MÍNIMO Y EL MÁXIMO PARAMOS LOS MOTORES.
        digitalWrite(motor_X_A, LOW);
        digitalWrite(motor_X_B, LOW);
        bucle = 0;
        return;
      }
    }

    if (activador_motor == 0) {   //SI LA FUNCION TEMPORIZADOR(); NOS DEVUELVE UN 0, APAGAMOS LOS MOTORES.
      digitalWrite(motor_X_A, LOW);
      digitalWrite(motor_X_B, LOW);
    }
  }
}

void DESPEGAR_GEARMOTOR(int motor_X_A, int motor_X_B) {
  pinesMotores.digitalWrite(motor_X_A, HIGH);
  pinesMotores.digitalWrite(motor_X_B, LOW);
  delay(4500);
  pinesMotores.digitalWrite(motor_X_A, LOW);
  pinesMotores.digitalWrite(motor_X_B, LOW);

}

void JUNTAR_GEARMOTOR(int motor_X_A, int motor_X_B) {
  pinesMotores.digitalWrite(motor_X_A, LOW);
  pinesMotores.digitalWrite(motor_X_B, HIGH);
  delay(4000);
  pinesMotores.digitalWrite(motor_X_A, LOW);
  pinesMotores.digitalWrite(motor_X_B, LOW);

}

void GIRAR_SERVO(int servo, int gradosA, int gradosB) {
  int pos;
  //SI GRADOA ES MENOR QUE GRADOB SIGNIFICA QUE TENEMOS QUE IR AUMENTANDO LOS GRADOS PROGRESIVAMENTE.
  if (gradosA < gradosB ) {
    for (pos = gradosA; pos <= gradosB; pos += 1) {
      switch (servo) {
        case 1:
          SERVO_A.write(pos);
          break;
        case 2:
          SERVO_B.write(pos);
          break;
        case 3:
          SERVO_C.write(pos);
          break;
      }
      delay(TIEMPO_SERVOS);
    }
  }
  //SI GRADOA ES MAYOR QUE GRADOB SIGNIFICA QUE TENEMOS QUE IR DISMINUYENDO LOS GRADOS PROGRESIVAMENTE.
  if (gradosA > gradosB) {
    for (pos = gradosA; pos >= gradosB; pos -= 1) {
      switch (servo) {
        case 1:
          SERVO_A.write(pos);
          break;
        case 2:
          SERVO_B.write(pos);
          break;
        case 3:
          SERVO_C.write(pos);
          break;
      }
      delay(TIEMPO_SERVOS);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinesMotores.begin();
  Serial.println("[BLUE] INICIADO");

  SERVO_A.attach(9);
  SERVO_B.attach(10);
  SERVO_C.attach(11);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);

  pinesMotores.pinMode(0, OUTPUT);
  pinesMotores.pinMode(1, OUTPUT);
  pinesMotores.pinMode(2, OUTPUT);
  pinesMotores.pinMode(3, OUTPUT);
  pinesMotores.pinMode(4, OUTPUT);
  pinesMotores.pinMode(5, OUTPUT);
  pinesMotores.pinMode(6, OUTPUT);
  pinesMotores.pinMode(7, OUTPUT);
}

bool activador = 1;
void loop() {
  /*NOMENCLATURA:
     GIRAR_SERVO(SERVO, GRADOS INICIO, GRADOS FINAL);
     GIRAR_GEARMOTOR(PIN A DEL MOTOR, PIN B DEL MOTOR, PIN DEL POTENCIOMETRO CORRESPONDIENTE, VALOR MINIMO DEL POT, VALOR MAXIMO DEL POT);
     DESPEGAR_GEARMOTOR(PIN A DEL GEARMOTOR, PIN B DEL GEARMOTOR);
     JUNTAR_GEARMOTOR(PIN A DEL GEARMOTOR, PIN B DEL GEARMOTOR);
  */

  if (activador == 1) {
    //POSICION INICIAL
    GIRAR_SERVO(1, 180, 0);
    GIRAR_GEARMOTOR(4, 7, A2, 490, 491);
    delay(10000);
    //DESPLEGAMOS EL SERVO PARA QUE EL QBOT NARANJA GIRE.
    GIRAR_GEARMOTOR(4, 7, A2, 490, 491);
    GIRAR_SERVO(1, 0, 180);
    delay(10000);
    GIRAR_SERVO(1, 150, 0);
    delay(12000);
    //DESPEGAR LADO
    DESPEGAR_GEARMOTOR(6, 7);
    delay(7000);
    //JUNTAR LADO
    JUNTAR_GEARMOTOR(6, 7);
    GIRAR_GEARMOTOR(4, 7, A2, 714, 715);
    delay(2000);
    GIRAR_GEARMOTOR(4, 7, A2, 714, 715);
    GIRAR_SERVO(1, 0, 180);
    delay(2000);
    GIRAR_SERVO(1, 150, 180);
    DESPEGAR_GEARMOTOR(2, 3);
    delay(500);
    GIRAR_SERVO(1, 180, 0);
    JUNTAR_GEARMOTOR(2, 3);
  }

  //CON ESTE CÓDIGO PODREMOS CONTROLAR EL QBOT DE FORMA MANUAL (CON EL TECLADO)
  if (Serial.available() > 0) {
    char caracter;
    caracter = Serial.read();
    Serial.println(caracter);
    if (caracter == 'A') {
      GIRAR_SERVO(1, 180, 0);
    }
    if (caracter == 'B') {
      GIRAR_GEARMOTOR(4, 7, A2, 490, 491);
    }
    if (caracter == 'C') {    //DESPLEGAR SERVO
      GIRAR_SERVO(1, 0, 180);
    }

    if (caracter == 'D') {    //[BLUE] DESPEGAR LADO
      DESPEGAR_GEARMOTOR(6, 7);
    }
    if (caracter == 'E') {    //[BLUE] JUNTAR LADO
      JUNTAR_GEARMOTOR(6, 7);
    }
    if (caracter == 'F') {
      GIRAR_GEARMOTOR(4, 7, A2, 709, 710);
    }
    if (caracter == 'G') {    //[BLUE] DESPEGAR ARRIBA
      DESPEGAR_GEARMOTOR(2, 3);
    }
    if (caracter == 'H') {    //[BLUE] JUNTAR ARRIBA
      JUNTAR_GEARMOTOR(2, 3);
    }
  }
}


