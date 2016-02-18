import webiopi
import servocontrol

servo = servocontrol.ServoController("/dev/ttyACM0")

def setup():
    servo.home()

def loop():
    #learn how to bind js variables to servo
    if not servo.getErrors() == 0:
        return

def destroy():
    servo.closeServo()

@webiopi.macro
def setCamera(roll,pitch,yaw):
    servo.setAngle(0,int(roll))
    servo.setAngle(1,int(pitch))
