#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
#define DELAY   100

MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522
Servo servoMotor;

void setup() {
  Serial.begin(9600); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
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
                  if(compareArray(ActualUID,Usuario1)){
//                    Serial.println("Acceso concedido...");
                    Serial.print('a');
                    servoMotor.write(180);
                      delay(3000);
                      servoMotor.write(0);
                  }
                  else if(compareArray(ActualUID,Usuario2)){
//                    Serial.println("Acceso concedido...");
                    Serial.print('a');
                    servoMotor.write(180);
                      delay(3000);
                      servoMotor.write(0);
                  }
                  else
                    Serial.print('i');
                  
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();      
                  delay(DELAY);    
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
