from picamera.array import PiRGBArray 
from picamera import PiCamera 
import time 
import cv2 
import sys 
import imutils 
import RPi.GPIO as GPIO 
import OSC 
import shiftpi 
import threading 
 
#GPIO + servo control took from RPi Labs, for UOIT Students 
#spi.science.uoit.ca/lab/servo 
 
btnPin = 23 
servoX = 18 
 
#GPIO.setmode(GPIO.BOARD) 
GPIO.setup(servoX, GPIO.OUT) 
GPIO.setup(btnPin, GPIO.IN, pull_up_down=GPIO.PUD_UP) 
 
 
p = GPIO.PWM(servoX,50) 
 
p.start(2) 
p.ChangeDutyCycle(7.5) 
 
shiftpi.digitalWrite(1, shiftpi.LOW) 
 
 
client = OSC.OSCClient() 
client.connect(('192.168.2.102', 12345)) 
#receive_address = '127.0.0.1', 12346 
#server = OSC.ThreadingOSCServer(receive_address) 
 
#server.addDefaultHandlers() 
 
#def result(addr, tags, stuff, source): 
#    if addr=="/lights": 
#        print "test", stuff 
#    else:  
#        print "tt", stuff 
 
#server.addMsgHandler("/lights", result) 
 
#print "test"  
#st = threading.Thread(target=server.serve_forever) 
#st.start() 
 
 
msg = OSC.OSCMessage() 
msg.append('BTN')

def translate(value, leftMin, leftMax, rightMin, rightMax):
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    valueScaled = float(value - leftMin) / float(leftSpan)

    return rightMin + (valueScaled * rightSpan)

#Get user supplied values
cascPath = sys.argv[1]

# Create the haar cascade
faceCascade = cv2.CascadeClassifier(cascPath)

# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
camera.resolution = (160, 120)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(160, 120))

# allow the camera to warmup
time.sleep(0.1)
lastTime = time.time()*1000.0
bPressed = False
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):

    input_state = GPIO.input(btnPin)
    if input_state == False and bPressed == False:
        msg.setAddress("/buttonOn")
        client.send(msg)
        bPressed = True

    elif input_state == True and bPressed == True:
        msg.setAddress("/buttonOff")
        client.send(msg)
        bPressed = False


    # grab the raw NumPy array representing the image, then initialize the timestamp
        # and occupied/unoccupied text
    image = frame.array
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Detect faces in the image
    faces = faceCascade.detectMultiScale(
    gray,
    scaleFactor=1.1,
    minNeighbors=5,
    minSize=(30, 30),
    flags = cv2.cv.CV_HAAR_SCALE_IMAGE
    )
   # print time.time()*1000.0-lastTime," Found {0} faces!".format(len(faces))
    lastTime = time.time()*1000.0
    # Draw a rectangle around the faces
    for (x, y, w, h) in faces:
        cv2.circle(image, (x+w/2, y+h/2), int((w+h)/3), (255, 255, 255), 1)
        posx = translate(x, 0, 120, 12, 2.5)
        if posx < 4.5 or posx > 10:
            p.ChangeDutyCycle(7.5)
        else:
            p.ChangeDutyCycle(posx)
    # show the frame
    cv2.imshow("Frame", image)
    key = cv2.waitKey(1) & 0xFF

        # clear the stream in preparation for the next frame
    rawCapture.truncate(0)

        # if the `q` key was pressed, break from the loop
    if key == ord("q"):
 #       st.stop()
        break
