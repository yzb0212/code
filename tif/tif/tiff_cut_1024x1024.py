# -*- coding: utf-8 -*-
# written by YZB at 20190715
# 主要功能：读入指定路径下的所有tiff图像，将各个tiff图像分为1kx1k的小切片，存至指定路径中
# 输入：输入路径，输出路径，输出图像尺寸
# 输出：一堆jpg格式的图像

import cv2
import os
import numpy as np
#filllfile读进来之后直接就是numpy array格式的，直接就可以开始折腾
import tifffile as tiff
import matplotlib.pyplot as plt

### path
Input_path="/Users/yangzhibo/Desktop/tif/Images"
Output_path="/Users/yangzhibo/Desktop/tif/Reserve"
dirs = os.listdir(Input_path)
print('dirs',dirs)

###Output_size
Output_size=1024

#主函数
for file in dirs:
    Image_name=Input_path+'/'+file
    im_2015 = tiff.imread(Image_name)
    image_max=im_2015.max()
    image_min=im_2015.min()
    image_max=image_max.astype(np.float)    #数字转为float，默认float64
    image_min=image_min.astype(np.float)    #数字转为float，默认float64
    im_2015=im_2015.astype(np.float)        #图像转为float，默认float64

    muptiply_num=image_max/255          #将原始图像放缩至0-255，放缩倍数
    im_2015=im_2015/muptiply_num
    im_2015=im_2015.astype(np.uint8)    #放缩

    # cv2.imwrite(Output_path+'/'+'Original.jpg',im_2015) #显示原图，为了看8G的图才加的
    
    im_shape=im_2015.shape              #原图的长与宽
    width=int(im_shape[0])
    height=int(im_shape[1])
    print('Image_Shape_width:',int(im_shape[0]))
    print('Image_Shape_height:',int(im_shape[1]))
    times_wid=width/Output_size         #横着裁多少个
    times_hei=height/Output_size        #竖着裁多少个
    print('Times_wid:',times_wid)
    print('Times_hei:',times_hei)
    for i in range(1,times_wid+1,1):
        for j in range(1,times_hei+1,1):
            name_i="%05d" % i
            name_j="%05d" % j
            name_i=str(name_i)
            name_j=str(name_j)
            cv2.imwrite(Output_path+'/'+str(file[:-4])+'_wid_'+name_i+'_hei_'+name_j+'.jpg',im_2015[(i-1)*Output_size:i*Output_size-1,(j-1)*Output_size:j*Output_size-1])
            print('wid_start',(i-1)*Output_size)
            print('wid_finish',i*Output_size-1)
