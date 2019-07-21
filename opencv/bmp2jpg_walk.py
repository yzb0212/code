# -*- coding: utf-8 -*-
# written by YZB at 好久以前
# 主要功能：bmp2jpg
# 输入：输入路径，输出路径，输出图像尺寸
# 输出：一堆jpg格式的图像
import cv2
import os

path_bmp='/Users/yangzhibo/Desktop/picture'
path_jpg='/Users/yangzhibo/Desktop/picture_change'
i=522

for root,dirs,BMP_PATHS in os.walk(path_bmp):
  for test_path in BMP_PATHS:
    print(test_path)
    img=cv2.imread(path_bmp+'/'+test_path)
    print('path_bmp+/+test_path',path_bmp+'/'+test_path)
    img_name=test_path[:-3]
    img_name.replace('.','_')
    # cv2.imshow('img',img)
    # cv2.waitKey(0)
    i=i+1
    name_change="%05d"%i
    cv2.imwrite(path_jpg+'/'+name_change+'.jpg',img)
    print('path_bmp+/+test_path+jpg',path_jpg+'/'+test_path[:-3]+'jpg')
