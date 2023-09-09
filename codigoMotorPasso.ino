#include <Arduino.h>
#include <MsTimer2.h>
#include <Thread.h>

unsigned char tcount = 0;
unsigned char Buffer[80];
unsigned char Buffer_Len = 0;

#define trigPin 12 
#define echoPin 13
#define buzzer 11
#define pinVerde 8 
#define pinAmarelo 10
#define pinVermelho 4
#define contage_prox 0x53  //definindo a conexão com a tela
#define distance_cm 0x52   //definindo a conexão com a tela

int contageProx;
int distanceCm; 

unsigned char contage1[8] = { 0x5a, 0xa5, 0x05, 0x82, contage_prox, 0x00, 0x00, 0x00 };
unsigned char distance2[8] = { 0x5a, 0xa5, 0x05, 0x82, distance_cm, 0x00, 0x00, 0x00 };

Thread myThread = Thread();

int buttonPin = 7;  
int ledPin = 2;    
int ledState = LOW;       
int buttonState;          
int lastButtonState = LOW;


void MotorInterrupt() {
  if (tcount > 0)
    tcount--;
}

void niceCallback(){
  long duration; 
  digitalWrite(trigPin, LOW); // pino trig do sensor começa desligado
  delayMicroseconds(2); //tempo de 2 microssegundos
  digitalWrite(trigPin, HIGH); // pino trig do sensor liga
  delayMicroseconds(10); //tempo de 10 microssegundos
  digitalWrite(trigPin, LOW); // pino trig desliga
  duration = pulseIn(echoPin, HIGH); // pino echo liga
  distanceCm = duration / 28.1 * 2;// conversão de distancia para centimentos
   
  /* configuração para demonstrar a distância em centímetros na tela */
  distance2[6] = highByte(distanceCm); 
  distance2[7] = lowByte(distanceCm);
  Serial.write(distance2,8);
  
  contage1[7] = lowByte(contageProx);
  if (distanceCm <= 20){
    delay(500);
    contage1[6] = highByte(contageProx);
    Serial.write(contage1,8);
    delay(500);
    contageProx =  contageProx + 1;
    contage1[7] = lowByte(contageProx);
   
    }    
  if (contageProx % 5 == 1){ 
   digitalWrite (pinAmarelo, HIGH);//acende o led amarelo
   delay (300);  // um curto período de pausa 
 
  }
   else { // caso não
   digitalWrite (pinAmarelo, LOW); //desliga o led amarelo
   } 
    

if (contageProx == 21){ // caso a contagem seja igual a 
   digitalWrite (pinVerde, HIGH);// acende o led verde
   delay (100);//um curto período de pausa
    } 
    else { // caso não
    digitalWrite (pinVerde, LOW); //desliga o led verde
    
}

if (contageProx == 21){ // caso a contagem seja igual a 
  
  contage1[7] = lowByte(contageProx);//pausa a contagem
  contageProx = 0;// reiniciará a contagem do zero
}

  if (distanceCm >= 25) { //caso distancia seja maior ou igual a 20 centimentros
   digitalWrite (pinVermelho, HIGH); //led vermelho liga
  
   digitalWrite (pinVerde, LOW); // led verde desliga
   digitalWrite (pinAmarelo, LOW);// led amarelo desliga
    delay (100); // tempo de 3 segundos
  //  contage1[7] = lowByte(contageProx); // pausa a contagem dos produtos passados
    contageProx = 0; // reinicia a contagem do zero
    digitalWrite(buzzer, HIGH);
     delay (100); // tempo de 1 segundo 
  }
   else { // caso nao
   digitalWrite (pinVermelho, LOW); // o led vermelho continuara desligado
   delay (100); // tempo de 1 segundo 
    digitalWrite(buzzer, LOW);
     delay (100); // tempo de 1 segundo 
   } 
 // Aguardar 100ms antes da próxima leitura para evitar interferência
 delay(100); 
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);    // Configura o pino do botão como entrada
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(pinVerde, OUTPUT); 
  pinMode(pinAmarelo, OUTPUT);
  pinMode(pinVermelho, OUTPUT);

  myThread.onRun(niceCallback);
  myThread.setInterval(100);


  MsTimer2::set(5, MotorInterrupt);
  MsTimer2::start();

  }

void loop(){


 esteira();
 if(myThread.shouldRun())
    myThread.run();

}
