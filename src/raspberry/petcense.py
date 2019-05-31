#usr/bin/python
#encode *utf-8*
#Hackathon honeywell

import serial
import tofb
from datetime import datetime

global fed
fed = False

def setupSerial(portName):
    return serial.Serial(port= portName, baudrate= 9600)

def leer_serial(serial):
    return serial.read(1)

def escribir_serial(serial, data):
    serial.write(data)


def checkHour(date):
    fed_morining_start = datetime(datetime.now().year, datetime.now().month, datetime.now().day, 15)
    fed_morining_finish = datetime(datetime.now().year, datetime.now().month, datetime.now().day, 16)
    fed_afternoon_start = datetime(datetime.now().year, datetime.now().month, datetime.now().day, 17)
    fed_afternoon_finish = datetime(datetime.now().year, datetime.now().month, datetime.now().day, 18)

    if (fed_morining_start < date < fed_morining_finish) or (fed_afternoon_start < date < fed_afternoon_finish):
        return True
    else:
        fed = False 
        return False


def acceso(ser, db):
    if ser.inWaiting() == 1:
        data = leer_serial(ser)
        if data == 'a':
            tofb.Set_hist_Acceso(db, 'Salio')
        if data == 'b':
            tofb.Set_hist_Acceso(db, 'Entro')
        
        
    
def alimentacion(ser, db):
    if ser.inWaiting() > 0:
        data = leer_serial(ser)
        if data == 'c':
            if checkHour(datetime.now()):
                escribir_serial(ser, b'y/n/r')
                fed = True
                tofb.Set_hist_Alimentacion(db)
                #tofb.sendNotification('Alimentacion', 'Tu mascota quiere comer')

            else:
                print('No es hora')
                escribir_serial(ser, b'n/n/r')
                #cont += 1
                #if cont > 10:
                 #   tofb.send_notificacion('Alimentacion', 'Tu mascota quiere comer')
            
def var_hora():
    return datetime.now().strftime("%Y-%m-%d_%H-%M")

