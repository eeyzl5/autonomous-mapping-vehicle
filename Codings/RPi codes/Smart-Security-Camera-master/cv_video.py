# import the necessary packages
from imutils.video import VideoStream
from camera import VideoCamera
import datetime
import argparse
import imutils
import time
import cv2

video_camera = VideoCamera(flip=True) 
object_classifier = cv2.CascadeClassifier("models/facial_recognition_model.xml")
 
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-p", "--picamera", type=int, default=-1,
	help="whether or not the Raspberry Pi camera should be used")
args = vars(ap.parse_args())
 
# initialize the video stream and allow the cammera sensor to warmup
#vs = VideoStream(usePiCamera=True).start()
time.sleep(2.0)

# loop over the frames from the video stream
while True:
	# grab the frame from the threaded video stream and resize it
	# to have a maximum width of 400 pixels
	#frame = vs.read()
	#frame = imutils.resize(frame, width=400)
 
	# draw the timestamp on the frame
	#timestamp = datetime.datetime.now()
	#ts = timestamp.strftime("%A %d %B %Y %I:%M:%S%p")
	#cv2.putText(frame, ts, (10, frame.shape[0] - 10), cv2.FONT_HERSHEY_SIMPLEX,
	#	0.35, (0, 0, 255), 1)
 
	# show the frame
	#cv2.rectangle(frame, (100, 100), (10, 10), (0, 255, 0), 2)
	frame, found_obj = video_camera.get_object(object_classifier)
	#edges = cv2.Canny(frame,40,80)
	cv2.imshow("Face Detection", frame)
	key = cv2.waitKey(1) & 0xFF
 
	# if the `q` key was pressed, break from the loop
	if key == ord("q"):
		break
 
# do a bit of cleanup
cv2.destroyAllWindows()
vs.stop()
