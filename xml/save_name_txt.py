# coding=utf-8
#计算给定目录下文件个数和文件夹的个数，将该文件夹下所有的文件
#在Python中，文件操作主要来自os模块，主要方法如下：
#os.getcwd()：获得当前工作目录
#os.path.isdir(name):判断name是不是一个目录，name不是目录就返回false
#os.path.isfile(name):判断name是不是一个文件，不存在name也返回false
#os.listdir(dirname)：列出dirname下的目录和文件
#os.path.join(path,name):连接目录与文件名或目录
 
import os
 
def walkFolders(folder):
    foldersCount = 0
    filesCount = 0
    folders = os.listdir(folder)
    rootdir=folder
    path_list=[]
    with open('name.txt','w')as f: #在当前folder目录下保存txt文件，只存文件，不存路径
        for item in folders:        
            curname = os.path.join(folder,item)
            if os.path.isdir(curname):
                foldersCount = foldersCount + 1
                path_list.extend(get_all_path(curname))
            elif os.path.isfile(curname):
                filesCount  = filesCount + 1
                path_list.append(curname)
                f.write(str(curname))  #把当前的文件名称写入txt文件
                f.write('\n')
    return filesCount,foldersCount,path_list


if __name__ == "__main__":
    #curFolder = os.getcwd()  
    curFolder="./picture_change" #输入文件夹路径
    filesCount,foldersCount,path_list = walkFolders(curFolder)
    print(filesCount)     #filesCount为文件个数
    print(foldersCount)   #foldersCount为文件夹个数
    print(path_list)      #当前目录下所有文件的一个list，和输出的txt文件中的东西是一样的
