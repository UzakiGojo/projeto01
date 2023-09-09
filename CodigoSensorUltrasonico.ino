#include <Arduino.h> //incluindo biblioteca
#include <MsTimer2.h>//incluindo biblioteca
#include <Wire.h>    //incluindo biblioteca
#include <Ultrasonic.h> //incluindo biblioteca

#define distance_cm      0x52//definindo a conexão com a tela
#define contage_prox     0x53//definindo a conexão com a tela

/* Quadro de dados para todos os sensores */
unsigned char   distance2[8]={0x5a, 0xa5, 0x05, 0x82, distance_cm, 0x00, 0x00, 0x00};
unsigned char   contage1[8]={0x5a, 0xa5, 0x05, 0x82, contage_prox, 0x00, 0x00, 0x00};

unsigned char tcount = 0;
unsigned char Buffer[80];
unsigned char Buffer_Len = 0;

int ledState = LOW;       
int buttonState;          
int lastButtonState = LOW;
long duration; //duração
int distanceCm; //declarando a variavel que vai armazenar a distancia em centimetros mostradas no display
int contageProx; // declarando a variavel que armazenara a contagem dos produtos mostradas na interface
long duracao; // definindo duração
int distancia; //guardando a disancia do sensor que ligara o led vermelho se ocorrer algum erro

const int echoPin = 13;  //definindo pino
const int trigPin = 12; //definindo pino
const int buzzer = 11; //definindo pino
int pinAmarelo = 10; // definindo o pino do led amarelo
int pinVerde = 9; // definindo o pino do led verde
const int ledPin = 8;   
const int buttonPin = 7;  
const int echoPino = 5;  //definindo pino
const int trigPino = 4; //definindo pino
int pinVermelho = 2; //definindo o pino do led vermelho

void Timer2Interrupt() {
  if (tcount > 0)
    tcount--;
}


void setup() {
  
 Serial.begin(115200);
 
  setup_esteira();
  
  
 MsTimer2::set(5, Timer2Interrupt );
 MsTimer2::start( ); 

{delay(100);}
 setup_sensor();


  
 
}
void loop() {
if(millis()>100){
  loop_esteira();
}
  if(millis()>101){
  loop_sensor1();
  }
  if(millis()>102){ 
  loop_sensor2();
  }
 if(millis()>103){
  contagem_produto();
 }
}
