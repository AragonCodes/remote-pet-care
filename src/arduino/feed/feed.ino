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
 * Servo = 2
 * sensor afuera = 5
 * sensor adentro = 6
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522

MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
Servo servoMotor;

void setup() {
  Serial.begin(9600); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  servoMotor.attach(2);
//  Serial.println("Control de acceso:");
}

byte ActualUID[4]; //almacenará el código del Tag leído
byte Usuario1[4]= {0x79, 0x65, 0xCB, 0x35} ; //código del usuario 1 79 65 CB 35
byte Usuario2[4]= {0x45, 0x03, 0x00, 0xAB} ; //código del usuario 2
int wait_answer = 0;
int incomingByte = 0;
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
                    Serial.println('c');
                    wait_answer = 1;
                    
                  }
                  else{
                    Serial.println('i');
                  }
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();                 
          
            }
        }

        while(wait_answer == 1){
            if (Serial.available() > 0) {
                    incomingByte = Serial.read();
                    if(incomingByte=='y'){
                      servoMotor.write(180);
                      delay(3000);
                      servoMotor.write(0);
                    } else if (incomingByte=='n'){
                      // Nothing -> "Not yet doggie"
                    } else{
                      
                    }
                    wait_answer = 0;
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
