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

    
    def setSpeed(self,servo,speed):
        #speed is accepted in microseconds/milliseconds. See pg 42 of documentation
        speed = speed / 0.025
        speed_low = (speed & 0x7f)
        speed_high = (speed >> 7)& 0x7f
        chan = servo & 0x7f

        if self.protocol=="pololu":
            data =  chr(0xaa) + chr(0x0c) + chr(0x07) + chr(chan) + chr(speed_low) + chr(speed_high)
        else:
            data = chr(0x87) + chr(chan) + chr(speed_low) + chr(speed_high)
        self.sc.write(data)

    def setAcceleration(self,servo,acc):
        #acceleration setting in microseconds/(milliseconds)^2
        #range of acc: 0 - 0.08 , if given less than 0.08
        #            : 0 - 255, if more than 0.08
        if acc < 0.08:
            acc = acc / 0.025 / 80
        else:
            acc = int(acc)
        
        if acc < 0:
            acc = 0
        elif acc > 255:
            acc = 255

        acc_low = (acc & 0x7f)
        acc_high = (acc >> 7) & 0x7f
        chan = servo & 0x7f

        if self.protocol=="pololu":
            data =  chr(0xaa) + chr(0x0c) + chr(0x09) + chr(chan) + chr(acc_low) + chr(acc_high)
        else:
            data = chr(0x89) + chr(chan) + chr(acc_low) + chr(acc_high)
        self.sc.write(data)


    def get_movingState(self):
        if self.protocol=="pololu":
            data =  chr(0xaa) + chr(0x0c) + chr(0x13)
        else:
            data = chr(0x93) 
        self.sc.write(data)
        if ord(self.sc.read()) == 0:
            return False #not moving
        else:
            return True #servos are moving


    def getPosition(self, servo):
        chan  = servo &0x7f
        data =  chr(0xaa) + chr(0x0c) + chr(0x10) + chr(chan)
        self.sc.write(data)
        w1 = ord(self.sc.read())
        w2 = ord(self.sc.read())
        pos = int(w1<<7) + int(w2)
        return pos

    def getErrors(self):
        data =  chr(0xaa) + chr(0x0c) + chr(0x21)
        self.sc.write(data)
        w1 = ord(self.sc.read())
        w2 = ord(self.sc.read())
        error_code = int(w1<<7) + int(w2)
        return error_code

    def triggerScript(self, subNumber):
        if self.protocol=="pololu":
            data =  chr(0xaa) + chr(0x0c) + chr(0x27) + chr(subNumber)
        else:
            data = chr(0xa7) + chr(subNumber)
        self.sc.write(data)

    def triggerScript_params(self,subNumber,parameter):
        param_lo = parameter & 0x7f
        param_hi = (parameter >> 7) & 0x7f

        if self.protocol=="pololu":
            data =  chr(0xaa) + chr(0x0c) + chr(0x28) + chr(subNumber) + chr(param_lo) + chr(param_hi)
        else:
            data = chr(0xa8) + chr(subNumber) + chr(param_lo) + chr(param_hi)

        self.sc.write(data)
        
    def stopScript(self):
        if self.protocol=="pololu":
            data =  chr(0xaa) + chr(0x0c) + chr(0x24)
        else:
            data = chr(0xA4) 
        self.sc.write(data)

    def getScriptStatus(self):
        if self.protocol=="pololu":
            data =  chr(0xaa) + chr(0x0c) + chr(0x2e)
        else:
            data = chr(0xae) 
        self.sc.write(data)
        if ord(self.sc.read()) == 1:
            return True #script is running
        else:
            return False #no scripts running


servo1=ServoController("/dev/ttyACM0")

#Send Servo Angle in degrees (0-180)
#servo1.setAngle(0,0)
#servo1.setAngle(1,0)

#Acceleration setting
# servo1.setAcceleration(0,10)
# servo1.setAcceleration(1,1)

#set Position works in 1/4th the position wanted. So, if 800 is req position, send 200
#Range : - 1000-2000
# servo1.setTarget(0,2000) 
# servo1.setTarget(1,1000)

#Returns a number
#print servo1.get_movingState()
#print servo1.getPosition(0)
#print servo1.getErrors()
servo1.closeServo()