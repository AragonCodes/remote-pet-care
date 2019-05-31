#import Libraries
import RPi.GPIO as GPIO
import time
import pyrebase
import smbus
import Adafruit_DHT
import requests
import datetime
import socket
import serial



bus = smbus.SMBus(1)    
DEVICE_ADDRESS = 0x08      #7 bit address (will be left shifted to add the read write bit)
DEVICE_REG_MODE1 = 0x00
DEVICE_REG_LEDOUT0 = 0x48
arduino=0x08


def get_ip_address():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    return s.getsockname()[0]

def startService():
    import os
    os.system("sudo service motion start")

def stopService():
    import os
    os.system("sudo service motion stop")
    
def sendNotification(text1, text2):
    text1 = text1.replace(" ", "%20")
    text2 = text2.replace(" ", "%20")
    requests.get("http://tongue-lash-adverbs.000webhostapp.com/Notification.php?title={}&msg={}".format(text1, text2))
    time.sleep(2)

def stream_handler(message):
 #    print(message["event"]) # put
    print(message["path"]) # /Alarma
    print(message["data"]) # {'Estado': 'false', 'odigo': '1234'}

    if (message["path"] == "/Luces/luz1"):
        if(message["data"] == '"false"'):
            print("Luz 1 apagada")
            estadoLuz1='"false"'
            ser.write(b'4')
        elif(message["data"] == '"true"'):
            print("Luz 1 prendida")
            estadoLuz1='"true"'
            ser.write(b'3')
    if (message["path"] == "/Luces/luz2"):
        if(message["data"] == '"false"'):
            print("Luz 2 apagada")
            estadoLuz2='"false"'
            ser.write(b'6')
        else:
            print("Luz 2 prendida")
            estadoLuz2='"true"'
            ser.write(b'5')

    if (message["path"] == "/Alarma/Estado"):
        if(message["data"] == '"false"'):                  
            print("Alarma Apagada")
            ser.write(b'a')
            Var_Sensores=False
            Var_Alarma=False
        elif(message["data"] == '"true"'):
            print("Alarma prendida")
            ser.write(b'b')
   
    if (message["path"] == "/Camara"):
        if(message["data"] == '"false"'):
            print("Camara Detenida")
            stopService()
        elif(message["data"] == '"true"'):
            print("Inicio de Camara")
            startService()

    if (message["path"] == "/Temperatura/Requested"):
        if(message["data"] == '"1"'):
            ser.write(b'7')
    if (message["path"] == "/autoluz"):
        if(message["data"] == '"false"'):
            ser.write(b'8')
        if(message["data"] == '"true"'):
            ser.write(b'9')
            
#Firebase Configuration
config = {
  "apiKey": "AIzaSyBgqXyMUrXnMb9FMp07yuSrRkbBwcU7mzQ",
  "authDomain": "domoticdb-2841e.firebaseapp.com",
  "databaseURL": "https://domoticdb-2841e.firebaseio.com",
  "storageBucket": "domoticdb-2841e.appspot.com"
}

firebase = pyrebase.initialize_app(config)

#GPIO Setup
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(16, GPIO.IN)  #Estado de la alarma
GPIO.setup(20, GPIO.IN)  #Estado de Luz1
GPIO.setup(21, GPIO.IN)  #Estado de Luz2
GPIO.setup(12, GPIO.IN)  #Estado Sensores

#Firebase Database Intialization
db = firebase.database()
##sensor=Adafruit_DHT.DHT11
##PinSensor=4 #gpio 17

estadoLuz1=db.child("Luces/luz1").get()
estadoLuz2=db.child("Luces/luz2").get()
estadoAlarma=db.child("Alarma/Estado").get()
#db.child().set({"ip": "{}".format(get_ip_address())})
print("{}".format(get_ip_address()))
      
Var_Sensores=False
Var_Alarma=False

ser=serial.Serial('/dev/ttyUSB0',9600)

my_stream = db.child("").stream(stream_handler)
try:
    while(True):

       #humidity, temperature = Adafruit_DHT.read(sensor, PinSensor)
        if (ser.inWaiting() > 0):
            rec = ser.read(ser.inWaiting())
            print(rec)
            data=rec.decode('utf-8')
            if (b'sensor' in rec):
                sendNotification("Detección de sensores!", "Precione para iniciar")
            if (rec==b'alarma\r\n'):
                sendNotification("Alarma Activada!", "Precione para iniciar")
            if (rec==b'L1\r\n'):
                db.child("Luces").update({"luz1": '"true"'})
            if (rec==b'L2\r\n'):
                db.child("Luces").update({"luz2": '"true"'})
            if (b't:' in rec):
                tmp = data[data.find("t:")+2:data.find(" ")]
                db.child("Temperatura").update({"Temp": '{}'.format(tmp)})
                hum = data[data.find("h:")+2:data.find("\r")]
                db.child("Temperatura").update({"Hum": '{}'.format(hum)})        
except KeyboardInterrupt:
    my_stream.close()
    ser.close()
    
    print("Everything Closed!")



