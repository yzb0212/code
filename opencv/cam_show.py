import cv2

videoCapture=cv2.VideoCapture(0)
success,frame=videoCapture.read()
num=1

while success:
    img=frame
    gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

    cv2.imshow('hi', img)
    success, frame = videoCapture.read()
    k=cv2.waitKey(1)
    if k==ord('q'):
        break
    elif k==ord('s'):
        cv2.imwrite(str(num)+'.jpg',frame)
        num+=1

videoCapture.release()
cv2.destroyAllWindows()