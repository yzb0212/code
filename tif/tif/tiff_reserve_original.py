# -*- coding: utf-8 -*-

import cv2
import os
from xml.etree.ElementTree import ElementTree
import numpy as np
#filllfile读进来之后直接就是numpy array格式的，直接就可以开始折腾
import tifffile as tiff
import matplotlib.pyplot as plt

### path

path="/Users/yangzhibo/Desktop/tif/Images"
dirs = os.listdir(path)
print('dirs',dirs)

#主函数
for file in dirs:
    Image_name=path+'/'+file
    print(Image_name)
    im_2015 = tiff.imread(Image_name)

FILE_2015 = './Result_1.tif'
im_2015 = tiff.imread(FILE_2015)

#Image Shape
im_shape=im_2015.shape
print('Image_Shape:',im_shape)
print('type(im_2015)_start:',type(im_2015))

#print('image_max.type_before_convert:',im_2015.max().dtype)
image_max=im_2015.max()
image_min=im_2015.min()
image_max=image_max.astype(np.float)
image_min=image_min.astype(np.float)
#print('image_max.type_after_convert:',image_max.dtype)

#print('im_2015.type_before_convert:',im_2015.dtype)
im_2015=im_2015.astype(np.float)
#print('im_2015.type_after_convert:',im_2015.dtype)

muptiply_num=image_max/255
print('image_max/255:',image_max/255)

im_2015=im_2015/muptiply_num
#注意，下面的np.unit8一定要是uint8而不是int8，因为cv2.的好多函数也只能处理unint8的
im_2015=im_2015.astype(np.uint8)

#输出转置对称的图像
im_2015_transpose=im_2015.T

#直方图均衡化后的图像
im_2015_equ = cv2.equalizeHist(im_2015)

#自适应直方图均衡化后的图像
clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8,8))
im_2015_equ_auto = clahe.apply(im_2015)

#调整亮度和对比度
im_2015_bright_contrast_05_s10 = np.uint8(np.clip((0.5 * im_2015 - 10), 0, 255))
im_2015_bright_contrast_05_s0 = np.uint8(np.clip((0.5 * im_2015), 0, 255))
im_2015_bright_contrast_05_p10 = np.uint8(np.clip((0.5 * im_2015 + 10), 0, 255))

im_2015_bright_contrast_07_s10 = np.uint8(np.clip((0.7 * im_2015 - 10), 0, 255))
im_2015_bright_contrast_07_s0 = np.uint8(np.clip((0.7 * im_2015), 0, 255))
im_2015_bright_contrast_07_p10 = np.uint8(np.clip((0.7 * im_2015 + 10), 0, 255))

im_2015_bright_contrast_09_s10 = np.uint8(np.clip((0.9 * im_2015 - 10), 0, 255))
im_2015_bright_contrast_09_s0 = np.uint8(np.clip((0.9 * im_2015), 0, 255))
im_2015_bright_contrast_09_p10 = np.uint8(np.clip((0.9 * im_2015 + 10), 0, 255))

im_2015_bright_contrast_10_s10 = np.uint8(np.clip((1 * im_2015 - 10), 0, 255))
im_2015_bright_contrast_10_s0 = np.uint8(np.clip((1 * im_2015), 0, 255))
im_2015_bright_contrast_10_p10 = np.uint8(np.clip((1 * im_2015 + 10), 0, 255))

im_2015_bright_contrast_11_s10 = np.uint8(np.clip((1.1 * im_2015 - 10), 0, 255))
im_2015_bright_contrast_11_s0 = np.uint8(np.clip((1.1 * im_2015), 0, 255))
im_2015_bright_contrast_11_p10 = np.uint8(np.clip((1.1 * im_2015 + 10), 0, 255))

im_2015_bright_contrast_13_s10 = np.uint8(np.clip((1.3 * im_2015 - 10), 0, 255))
im_2015_bright_contrast_13_s0 = np.uint8(np.clip((1.3 * im_2015), 0, 255))
im_2015_bright_contrast_13_p10 = np.uint8(np.clip((1.3 * im_2015 + 10), 0, 255))

im_2015_bright_contrast_15_s10 = np.uint8(np.clip((1.5 * im_2015 - 10), 0, 255))
im_2015_bright_contrast_15_s0 = np.uint8(np.clip((1.5 * im_2015), 0, 255))
im_2015_bright_contrast_15_p10 = np.uint8(np.clip((1.5 * im_2015 + 10), 0, 255))

#中值滤波
im_2015_mid_filter_3 = cv2.medianBlur(im_2015,3)
im_2015_mid_filter_5 = cv2.medianBlur(im_2015,5)
im_2015_mid_filter_7 = cv2.medianBlur(im_2015,7)

#单纯blue
im_2015_blue_3 = cv2.blur(im_2015,(3,3))
im_2015_blue_5 = cv2.blur(im_2015,(5,5))
im_2015_blue_7 = cv2.blur(im_2015,(7,7))

print('image:',im_2015)
i1 = plt.imshow(im_2015)

cv2.imwrite('Result_1.jpg',im_2015)
cv2.imwrite('Result_1_transpose.jpg',im_2015_transpose)
cv2.imwrite('Result_1_equ.jpg',im_2015_equ)
cv2.imwrite('Result_1_equ_auto.jpg',im_2015_equ_auto)

cv2.imwrite('Result_1_bright_contrast_05_s10.jpg',im_2015_bright_contrast_05_s10)
cv2.imwrite('Result_1_bright_contrast_05_s0.jpg',im_2015_bright_contrast_05_s0)
cv2.imwrite('Result_1_bright_contrast_05_p10.jpg',im_2015_bright_contrast_05_p10)
cv2.imwrite('Result_1_bright_contrast_07_s10.jpg',im_2015_bright_contrast_07_s10)
cv2.imwrite('Result_1_bright_contrast_07_s0.jpg',im_2015_bright_contrast_07_s0)
cv2.imwrite('Result_1_bright_contrast_07_p10.jpg',im_2015_bright_contrast_07_p10)
cv2.imwrite('Result_1_bright_contrast_09_s10.jpg',im_2015_bright_contrast_09_s10)
cv2.imwrite('Result_1_bright_contrast_09_s0.jpg',im_2015_bright_contrast_09_s0)
cv2.imwrite('Result_1_bright_contrast_09_p10.jpg',im_2015_bright_contrast_09_p10)
cv2.imwrite('Result_1_bright_contrast_10_s10.jpg',im_2015_bright_contrast_10_s10)
cv2.imwrite('Result_1_bright_contrast_10_s0.jpg',im_2015_bright_contrast_10_s0)
cv2.imwrite('Result_1_bright_contrast_10_p10.jpg',im_2015_bright_contrast_10_p10)
cv2.imwrite('Result_1_bright_contrast_11_s10.jpg',im_2015_bright_contrast_11_s10)
cv2.imwrite('Result_1_bright_contrast_11_s0.jpg',im_2015_bright_contrast_11_s0)
cv2.imwrite('Result_1_bright_contrast_11_p10.jpg',im_2015_bright_contrast_11_p10)
cv2.imwrite('Result_1_bright_contrast_13_s10.jpg',im_2015_bright_contrast_13_s10)
cv2.imwrite('Result_1_bright_contrast_13_s0.jpg',im_2015_bright_contrast_13_s0)
cv2.imwrite('Result_1_bright_contrast_13_p10.jpg',im_2015_bright_contrast_13_p10)
cv2.imwrite('Result_1_bright_contrast_15_s10.jpg',im_2015_bright_contrast_15_s10)
cv2.imwrite('Result_1_bright_contrast_15_s0.jpg',im_2015_bright_contrast_15_s0)
cv2.imwrite('Result_1_bright_contrast_15_p10.jpg',im_2015_bright_contrast_15_p10)

cv2.imwrite('Result_1_mid_filter_3.jpg',im_2015_mid_filter_3)
cv2.imwrite('Result_1_mid_filter_5.jpg',im_2015_mid_filter_5)
cv2.imwrite('Result_1_mid_filter_7.jpg',im_2015_mid_filter_7)

cv2.imwrite('Result_1_blue_3.jpg',im_2015_blue_3)
cv2.imwrite('Result_1_blue_5.jpg',im_2015_blue_5)
cv2.imwrite('Result_1_blue_7.jpg',im_2015_blue_7)
####################write#################

#plt.show()
