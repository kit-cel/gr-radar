from cv2 import VideoCapture, imwrite
from datetime import datetime

cam = VideoCapture(0) # 0 -> index of camera
s, img = cam.read()
if s:
	tm = datetime.now()
	filename = "date-"+str(tm.year)+"-"+ str(tm.month)+"-"+ str(tm.day)+"_time-"+ str(tm.hour)+"-"+ str(tm.minute)+"-"+ str(tm.second)+"-"+str(tm.microsecond)+".jpg"
	imwrite(filename,img)
