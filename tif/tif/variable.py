# -*- coding: utf-8 -*-
# 变量名字中用变量的值

import cv2
import os
from xml.etree.ElementTree import ElementTree
import numpy as np
#filllfile读进来之后直接就是numpy array格式的，直接就可以开始折腾
import tifffile as tiff
import matplotlib.pyplot as plt


for i in range(5,15,1):
    print(i)
    j = "%03d" % i
    print(j)
    locals()['fuck_'+str(j)]=i*i
    print('fuck_'+str(j))
    #print 'fuck_'+str(j)
    print(locals()['fuck_'+str(j)])
