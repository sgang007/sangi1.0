import webiopi
#from webiopi.devices.serial import Serial
from serial import Serial
import time


class ServoController:
    def __init__(self, port , protocol="compact"):
        #protocol can be 'compact' or 'pololu' 
        self.protocol = protocol
        serPort = port
        self.sc = Serial(serPort)

    def closeServo(self):
        self.sc.close()

    
    def setAngle(self, n, angle):
        #setAngle is setTarget is miniSSC protocol
        if angle > 180 or angle <0:
           angle=90
        byteone=int(254*angle/180)
        bud=chr(0xFF)+chr(n)+chr(byteone)
        self.sc.write(bud)

    def setTarget(self, servo, position):
        position = position * 4
        poslo = (position & 0x7f)
        poshi = (position >> 7) & 0x7f
        chan  = servo &0x7f
        # Using the Pololu Protocol
        if self.protocol=="pololu":
            data =  chr(0xaa) + chr(0x0c) + chr(0x04) + chr(chan) + chr(poslo) + chr(poshi)
        else:
            data = chr(0x84) + chr(chan) + chr(poslo) + chr(poshi)
        self.sc.write(data)

    
    
    def getPosition(self, servo):
        chan  = servo &0x7f
        data =  chr(0xaa) + chr(0x0c) + chr(0x10) + chr(chan)
        self.sc.write(data)
        w1 = ord(self.sc.read())
        w2 = ord(self.sc.read())
        return w1, w2

    def getErrors(self):
        data =  chr(0xaa) + chr(0x0c) + chr(0x21)
        self.sc.write(data)
        w1 = ord(self.sc.read())
        w2 = ord(self.sc.read())
        return w1, w2

    def triggerScript(self, subNumber):
        data =  chr(0xaa) + chr(0x0c) + chr(0x27) + chr(subNumber)
        self.sc.write(data)

servo1=ServoController("/dev/ttyACM0")

#Send Servo Angle in degrees (0-180)
#servo1.setAngle(0,0)
#servo1.setAngle(1,0)

#set Position works in 1/4th the position wanted. So, if 800 is req position, send 200
#Range : - 1000-2000
servo1.setTarget(0,2000) 
servo1.setTarget(1,2000)

#Returns a number
#print servo1.getPosition(0)
#print servo1.getErrors()
servo1.closeServo()