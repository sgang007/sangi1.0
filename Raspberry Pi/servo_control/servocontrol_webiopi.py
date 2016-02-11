import webiopi
from webiopi.devices.serial import Serial
import time


class ServoController:
    def __init__(self, port, BAUD_RATE):
        serPort = port
        self.sc = Serial(serPort,BAUD_RATE)

    def closeServo(self):
        self.sc.close()

    def setAngle(self, n, angle):
        if angle > 180 or angle <0:
           angle=90
        byteone=int(254*angle/180)
        bud=chr(0xFF) + chr(n) + chr(byteone)
        self.sc.writeString(bud)

    def setPosition(self, servo, position):
        position = position * 4
        poslo = (position & 0x7f)
        poshi = (position >> 7) & 0x7f
        chan  = servo &0x7f
        data =  chr(0xaa) + chr(0x0c) + chr(0x04) + chr(chan) + chr(poslo) + chr(poshi)
        self.sc.writeString(data)

    def getPosition(self, servo):
        chan  = servo &0x7f
        data =  chr(0xaa) + chr(0x0c) + chr(0x10) + chr(chan)
        self.sc.writeByte(data)
        w1 = ord(self.sc.readString())
        w2 = ord(self.sc.readString())
        return w1, w2

    def getErrors(self):
        data =  chr(0xaa) + chr(0x0c) + chr(0x21)
        self.sc.write(data)
        w1 = ord(self.sc.readString())
        w2 = ord(self.sc.readString())
        return w1, w2

    def triggerScript(self, subNumber):
        data =  chr(0xaa) + chr(0x0c) + chr(0x27) + chr(0)
        self.sc.writeString(data)
GPIO = webiopi.GPIO
LIGHT=25

maestro = ServoController('/dev/ttyAMA0',9600)

def setup():
    GPIO.setFunction(LIGHT, GPIO.OUT)
    maestro.setAngle(0,90)
    maestro.setAngle(1,90)
    maestro.setAngle(2,90)    
    maestro.setAngle(3,90)
    maestro.setAngle(4,90)
    maestro.setAngle(5,90)

def loop():
    #TODO: Implement Kalman smoothing or other algos here
   # print "Servo Positions"
   # print maestro.getPosition(0),':',maestro.getPosition(1),':',maestro.getPosition(2),':',maestro.getPosition(3),':',maestro.getPosition(4),':',maestro.getPosition(5)
   # print '\n Errors: ', maestro.getErrors()
    if(GPIO.digitalRead(LIGHT)==GPIO.HIGH):
        GPIO.digitalWrite(LIGHT, GPIO.LOW)
    else:
        GPIO.digitalWrite(LIGHT,GPIO.HIGH)
    
    webiopi.sleep(100)

def destroy():
    maestro.closeServo()

def setAngle( n, angle):
    if angle > 180 or angle <0:
       angle=90
    byteone=int(254*angle/180)
    bud=chr(0xFF) + chr(n) + chr(byteone)
    sc = Serial('/dev/ttyAMA0',9600)
    sc.writeString(bud)

@webiopi.macro
def setCamera(roll,pitch,yaw):
    #TODO: Calculate servo angles here
    setAngle(0,45)
    maestro.setAngle(0,int(roll))
    maestro.setAngle(1,int(pitch))


