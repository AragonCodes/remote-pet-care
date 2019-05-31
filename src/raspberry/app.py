import petcense
import time
import tofb

db = tofb.config_firebase()

puerta = petcense.setupSerial('/dev/ttyACM0')
comida = petcense.setupSerial('/dev/ttyUSB0')

while True:

    petcense.acceso(puerta,db)
    petcense.alimentacion(comida, db)
    time.sleep(10)
    
