# -*- coding: utf-8 -*-
# written by YZB at 20190721
# 主要功能：更改标签文件中的<name>,如将所有的ship都改成boat
# 输入：输入路径，输出路径
# 输出：一堆更改名称后的xml
import os
from xml.etree.ElementTree import ElementTree


path="/Users/yangzhibo/Desktop/pic_GF2_Result_20190720/xml/"
path1="/Users/yangzhibo/Desktop/pic_GF2_Result_20190720/xml/"
dirs = os.listdir(path)

tree = ElementTree()

i=1

# 输出所有文件和文件夹
for file in dirs:
    print("file:",file)             #('file:', 'Result_wid_00001_hei_00303.xml')
    tree.parse(path+'/'+file)
    root = tree.getroot()
    print("root",root)              # ('root', <Element 'annotation' at 0x101626d50>)
    for obj in root.iter('object'): #读取root下面所有含‘object’obj
        name=obj.find("name").text  # ('obj.name_before', 'ship')
        print("obj",obj)            # ('obj', <Element 'object' at 0x101832cd0>)
        print("obj.name_before",obj.find("name").text)
        if name=='ship':
            print("fuck!!!")        # fuck！！
            obj.find("name").text='boat'   #更改为boat
        print("obj.name_after",obj.find("name").text)   #('obj.name_after', 'boat')
    print("root.tag:",root.tag)
    print("root[0].tag:",root[0].tag)
    print("root[1].tag:",root[1].tag)
    print("root[1].text:",root[1].text)
    print("root[2].tag:",root[2].tag)
    print("root[2].attrib:",root[2].attrib)           #不好使
    print("root[2].get(path):",root[2].get("path"))   #不好使
    print("root[2].text:",root[2].text)               #就这个是好使的
    # root[0].text='xml'
    # root[2].text=path1+root[1].text
    print("root[2].text_after_change:",root[2].text)   

    print("root[5].tag:",root[5].tag)    
    print("root[6].tag:",root[6].tag)        #object
    print("root[6].text:",root[6].attrib)   
    tree.write(path1+'/'+file)                        #最后必须要有这个，要不然只是显示，写不进去
    # root[2].text ='fuck'

    #j = "%05d" % i
    #j=str(j)
    #root[0].text='ship_change'
    #root[1].text=j+'.'+'jpg'
    #oot[2].text="/Users/yangzhibo/Desktop/remote_img/ship_change/"+j+'.'+'jpg'
    #i=i+1
    #tree.write(path1+'/'+file)