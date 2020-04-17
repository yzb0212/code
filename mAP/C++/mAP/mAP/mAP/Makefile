include ../define.opt

#依赖库存放路径及程序输出路径
ROOTPATH = ../x64/bin

#模块名,与文件夹名称一致
MODULENAME="mAP"

#动态库后缀为.so,静态库后缀为.a,编译可执行文件什么都不添加
LIBNAME = "lib$(MODULENAME).so"

#定义了一个宏
RM = rm -rf

#依赖库路径及需要链接的依赖库
LIBS = -L$(ROOTPATH) $(COMMONLIBS) 

#c编译选项
CFLAGS = $(FLAGS) -I"../$(MODULENAME)" -I"/usr/include" 

#c++编译选项
CPPFLAGS = $(FLAGS) -I"../$(MODULENAME)" -I"/usr/include"

#cu编译选项
CUFLAGS = $(FLAGS)

#库编译选项
OUTPUTFLAGS = -shared

#c
CSOURCE = $(wildcard *.c)
#cpp
CPPSOURCE = $(wildcard *.cpp)
#cu
CUSOURCE = $(wildcard *.cu)
#c  patsubst函数用于将文件模式进行替换,  格式：$(patsubst 原模式， 目标模式， 文件列表)
COBJ = $(patsubst %.c,%.o, $(CSOURCE)) 
#cpp
CPPOBJ = $(patsubst %.cpp,%.o, $(CPPSOURCE))
#cu
CUOBJ = $(patsubst %.cu,%.o, $(CUSOURCE))

default: $(COBJ) $(CPPOBJ) $(CUOBJ)
#静态库注释掉第一行,动态库注释掉第二行
	$(CPPC) $(OUTPUTFLAGS) -o "$(ROOTPATH)/$(LIBNAME)" $(COBJ) $(CPPOBJ) $(LIBS)
#	ar -r "$(ROOTPATH)/$(LIBNAME)" $(COBJ) $(CPPOBJ)
#	$(CUDAC) $(OUTPUTFLAGS) -o "$(ROOTPATH)/$(LIBNAME)" $(COBJ) $(CPPOBJ) $(CUOBJ) $(LIBS)

#%.o: %.cu
#	$(CUDAC) $(CUFLAGS)

%.o: %.cpp
	$(CPPC) $(CPPFLAGS)

%.o: %.c
	$(CC) $(CFLAGS)

clean:
	$(RM) *.o
	$(RM) *.d
	$(RM) "$(ROOTPATH)/$(LIBNAME)"
