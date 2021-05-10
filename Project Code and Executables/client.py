import cv2 
import time
import urllib.request
import numpy as np
import ssl
import requests
import serial

Arduinoserial = serial.Serial('COM5',9600)

def captureImage():
    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE

    url = 'http://192.168.0.7:8080/video' #link to IP webcam address goes here
    cap = cv2.VideoCapture(url) 
    ret, frame = cap.read() 
    cv2.imwrite('Image.JPG', frame) 
    q = cv2.waitKey(1)
    cap.release()
    cv2.destroyAllWindows()



    url = 'http://eed85aa23592.ngrok.io/' #link to ngrok address goes here

    filename = 'Image.JPG'

    file = {'file': open(filename, 'rb'),'Content-Type': 'image/jpeg'}

    readresponse = requests.post(url,files=file)
    print(readresponse.text)

    if(readresponse.text == 'plastic'):
        print("It is Plastic...")
        time.sleep(2)
        Arduinoserial.write(str.encode('1'))
    elif(readresponse.text == 'glass'):
        print("It is Glass...")
        time.sleep(2)
        Arduinoserial.write(str.encode('0'))
    else:
        print("Not detected...")


while(True):
    var = Arduinoserial.readline().decode()
    var = int(var)
    if(var == 1):
        start = time.time()
        print("Before Capture")
        captureImage()
        print("After Capture")
        end = time.time()
        print(end - start)
    Arduinoserial.flush()

