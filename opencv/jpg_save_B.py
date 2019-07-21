# -*- coding: utf-8 -*-
# written by YZB at 20190721
# 主要功能：保存RGB中的R，做一个假的全色图像
# 输入：输入路径，输出路径，输出图像尺寸
# 输出：一堆jpg格式的图像

import cv2
import os
import numpy as np
import matplotlib.pyplot as plt

### path
Input_path="/Users/yangzhibo/Desktop/pic_GF2_Result_20190720/Google_earth/pic/"
Output_path="/Users/yangzhibo/Desktop/pic_GF2_Result_20190720/Google_earth/pic/"
dirs = os.listdir(Input_path)
print('dirs',dirs)

###Output_size
Output_size=1024

#主函数
for file in dirs:
    Image_name=Input_path+'/'+file
    print("Image_name",Image_name)
    im_2015 = cv2.imread(Image_name)
    im_shape=im_2015.shape              #原图的长与宽
    width=int(im_shape[0])
    height=int(im_shape[1])
    depth=int(im_shape[2])
    print('Image_Shape_width_rgb:',int(im_shape[0]))
    print('Image_Shape_height_rgb:',int(im_shape[1]))
    print('Image_Shape_depth_rgb:',int(im_shape[2]))
    im_2015 = im_2015[:,:,1]
    im_shape=im_2015.shape              #原图的长与宽
    width=int(im_shape[0])
    height=int(im_shape[1])
    print('Image_Shape_width_rgb:',int(im_shape[0]))
    print('Image_Shape_height_rgb:',int(im_shape[1]))
    cv2.imwrite(Image_name,im_2015)
