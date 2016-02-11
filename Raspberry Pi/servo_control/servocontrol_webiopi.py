import webiopi
from servocontrol import ServoController

maestro = ServoController('/dev/ttyAMA0',9600)

def setup():
    maestro.setAngle(0,90)
    maestro.setAngle(1,90)
    maestro.setAngle(2,90)    
    maestro.setAngle(3,90)
    maestro.setAngle(4,90)
    maestro.setAngle(5,90)

def loop():
    #TODO: Implement Kalman smoothing or other algos here
    print "Servo Positions:- \n "
    print maestro.getPosition(0),':',maestro.getPosition(1),':',maestro.getPosition(2),':',maestro.getPosition(3),':',maestro.getPosition(4),':',maestro.getPosition(5)
    print '\n Errors: ', maestro.getErrors()
    webiopi.sleep(1)

def destroy():
    maestro.closeServo()

@webiopi.macro
def setCamera(roll,pitch,yaw):
    #TODO: Calculate servo angles here
    maestro.setAngle(0,roll)
    maestro.setAngle(1,yaw)
