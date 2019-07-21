# -*- coding: utf-8 -*-
# written by YZB at 20190715
# 主要功能：数据扩增，（原图+转置）x[（0.5：1.5:0.15）+equ+equ_auto]x(3xBlur+3xmedianBlue)
# 输入：输入路径，输出路径
# 输出：一堆扩增后的jpg格式的图像

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

#滤波，输入为各亮度对比度下图像，调用cv2.imwrite函数，无返回值
def filter(Img_input,Img_name):
    im_2015=Img_input
    #中值滤波
    print('Start processing '+Img_name)
    im_2015_mid_filter_3 = cv2.medianBlur(im_2015,3)
    im_2015_mid_filter_5 = cv2.medianBlur(im_2015,5)
    im_2015_mid_filter_7 = cv2.medianBlur(im_2015,7)
    
    #单纯blue
    im_2015_blue_3 = cv2.blur(im_2015,(3,3))
    im_2015_blue_5 = cv2.blur(im_2015,(5,5))
    im_2015_blue_7 = cv2.blur(im_2015,(7,7))
    
    cv2.imwrite(Output_path+'/'+str(Img_name)+'_mid_filter_3.jpg',im_2015_mid_filter_3)
    cv2.imwrite(Output_path+'/'+str(Img_name)+'_mid_filter_5.jpg',im_2015_mid_filter_5)
    cv2.imwrite(Output_path+'/'+str(Img_name)+'_mid_filter_7.jpg',im_2015_mid_filter_7)
    
    cv2.imwrite(Output_path+'/'+str(Img_name)+'_blue_3.jpg',im_2015_blue_3)
    cv2.imwrite(Output_path+'/'+str(Img_name)+'_blue_5.jpg',im_2015_blue_5)
    cv2.imwrite(Output_path+'/'+str(Img_name)+'_blue_7.jpg',im_2015_blue_7)

#改变亮度对比度，输入为原始与转置的uint8图像，调用滤波函数，无返回值
def brightness_contrast(Img_input,Img_name):
    im_2015_equ = cv2.equalizeHist(im_2015)
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8,8))
    im_2015_equ_auto = clahe.apply(im_2015)
    cv2.imwrite(Output_path+'/'+str(Img_name+'_equ')+'.jpg',im_2015_equ)              #输出equ原图
    cv2.imwrite(Output_path+'/'+str(Img_name+'_equ_auto')+'.jpg',im_2015_equ_auto)    #输出equ_auto原图
    filter(im_2015_equ,str(Img_name+'_equ'))
    filter(im_2015_equ_auto,str(Img_name+'_equ_auto'))
    for i in [0.55,0.7,0.85,1,1.16,1.3,1.45]:
        #print('i:',i)
        i_int=int(100*i)
        #print('i_int',i_int)
        locals()[Img_name+'_bri_contr_'+str(i_int)+'_s10']=np.uint8(np.clip((i * im_2015 - 10), 0, 255))
        locals()[Img_name+'_bri_contr_'+str(i_int)+'_s0']=np.uint8(np.clip((i * im_2015), 0, 255))
        locals()[Img_name+'_bri_contr_'+str(i_int)+'_p10']=np.uint8(np.clip((i * im_2015 + 10), 0, 255))
        #print(locals()[Img_name+'_bri_contr_'+str(i_int)+'_s10'])
        filter(locals()[Img_name+'_bri_contr_'+str(i_int)+'_s10'],str(Img_name+'_bri_contr_'+str(i_int)+'_s10'))
        filter(locals()[Img_name+'_bri_contr_'+str(i_int)+'_s0'],str(Img_name+'_bri_contr_'+str(i_int)+'_s0'))
        filter(locals()[Img_name+'_bri_contr_'+str(i_int)+'_p10'],str(Img_name+'_bri_contr_'+str(i_int)+'_p10'))

#主函数
for file in dirs:
    Image_name=Input_path+'/'+file
    print(Image_name)
    print(file[:-4]+'_T')
    im_2015 = tiff.imread(Image_name)
    image_max=im_2015.max()
    image_min=im_2015.min()
    image_max=image_max.astype(np.float)
    image_min=image_min.astype(np.float)
    im_2015=im_2015.astype(np.float)
    muptiply_num=image_max/255
    im_2015=im_2015/muptiply_num
    im_2015=im_2015.astype(np.uint8)
    cv2.imwrite(Output_path+'/'+str(file[:-4])+'_original.jpg',im_2015)       #输出原图
    cv2.imwrite(Output_path+'/'+str(file[:-4])+'_transpose.jpg',im_2015.T)    #输出转置后的图像
    brightness_contrast(im_2015,str(file[:-4]))
    brightness_contrast(im_2015.T,str(file[:-4]+'_T'))

