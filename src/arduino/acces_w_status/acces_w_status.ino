   /*
Pins  SPI    UNO  
1 (NSS) SAD (SS)   10  a  
2       SCK        13  ab 
3       MOSI       11  c  
4       MISO       12  cb 
5       IRQ        *   n   
6       GND       GND  nb  
7       RST        5   v   
8      +3.3V (VCC) 3V3 vb  
* Not needed
1 on ICPS header
*/

/*
 * Arduino Pins
 * Servo = 8
 * sensor afuera = 5
 * sensor adentro = 6
 */


#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>



#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
#define DELAY   100

MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
Servo servoMotor;

// Custom variables
int eval = 0;
int s_afuera = 5;
int s_adentro = 6;

void setup() {
  Serial.begin(9600); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  pinMode(s_afuera, INPUT);
  pinMode(s_adentro, INPUT);
  servoMotor.attach(8);
//  Serial.println("Control de acceso:");
}

byte ActualUID[4]; //almacenará el código del Tag leído
byte Usuario1[4]= {0x79, 0x65, 0xCB, 0x35} ; //código del usuario 1 79 65 CB 35
byte Usuario2[4]= {0x45, 0x03, 0x00, 0xAB} ; //código del usuario 2
void loop() {
  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  // Enviamos serialemente su UID
//                  Serial.print(F("Card UID:"));
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
//                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  } 
//                  Serial.print("     ");                 
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  if(compareArray(ActualUID,Usuario1)||compareArray(ActualUID,Usuario2)){
//                    Serial.println("Acceso concedido...");
                    eval = 1;
                    servoMotor.write(180);
                    
                  }
                  else{
                    Serial.println('i');
                  }
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();                 
          
            }
        }
        while(eval == 1){
            if(digitalRead(s_adentro)){
              Serial.print('a');  
              servoMotor.write(0); 
              eval = 0;                 
            } else if(digitalRead(s_afuera)){
              Serial.print('b');
              eval = 0;
              servoMotor.write(0);
            }
            
        }
}

//Función para comparar dos vectores
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}
