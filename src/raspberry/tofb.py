#import Libraries
import RPi.GPIO as GPIO
import time
import pyrebase
import requests
import datetime
import socket
import serial
import petcense
global db


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

def config_firebase():
    #Firebase Configuration
    config = {
      "apiKey": "AIzaSyDVLsHqEpgpI679wT2MS-tcFFG04axvgAA",
      "authDomain": "petcense.firebaseapp.com",
      "databaseURL": "https://petcense.firebaseio.com",
      "storageBucket": "petcense.appspot.com"
    }

    firebase = pyrebase.initialize_app(config)
    #Firebase Database Intialization
    return firebase.database()        

def Set_hist_Acceso(db,estado):
    tempHist=db.child("acceso/HistorialAcc").get()
    hora=petcense.var_hora()
    db.child("acceso").update({"HistorialAcc": tempHist.val()+"&"+str(hora)+"_"+estado})
    db.child("acceso").update({"status":estado})


def Set_hist_Alimentacion(db):
##    print("set history alimentacion")
    tempHist=db.child("Alimentacion/HistorialAl").get()
    hora=petcense.var_hora()
    db.child("Alimentacion").update({"HistorialAl": tempHist.val()+"&"+str(hora)})




