import numpy as np  
import matplotlib.pyplot as plt 

file_1 = open("SSD_384x216_carperson_full16_20180621.txt","r")  
list_arr_1 = file_1.readlines()  
file_2 = open("SSD_384x216_carperson_full16_depthwise_20180622.txt","r")
list_arr_2 = file_2.readlines()
file_3 = open("SSD_384x216_carperson_full16_shuffle_20180621.txt","r")
list_arr_3 = file_3.readlines()
file_4 = open("SSD_384x216_carperson_full16_shuffle_20180623.txt","r")
list_arr_4 = file_4.readlines()

lists_1 = []  
lists_2 = []  
lists_3 = []  
lists_4 = []  

for index_1,x_1 in enumerate(list_arr_1):  
    x_1 = x_1.strip()  
    x_1 = x_1.strip('[]')  
    x_1 = x_1.split(", ")  
    lists_1.append(x_1)  
a_1 = np.array(lists_1)  
a_1 = a_1.astype(float)  
print (a_1)  
file_1.close()  
print(a_1.shape)

for index_2,x_2 in enumerate(list_arr_2):
    x_2 = x_2.strip()
    x_2 = x_2.strip('[]')
    x_2 = x_2.split(", ")
    lists_2.append(x_2)
a_2 = np.array(lists_2)
a_2 = a_2.astype(float)
print (a_2)
file_2.close()
print(a_2.shape)

for index_3,x_3 in enumerate(list_arr_3):
    x_3 = x_3.strip()
    x_3 = x_3.strip('[]')
    x_3 = x_3.split(", ")
    lists_3.append(x_3)
a_3 = np.array(lists_3)
a_3 = a_3.astype(float)
print (a_3)
file_3.close()
print(a_3.shape)

for index_4,x_4 in enumerate(list_arr_4):
    x_4 = x_4.strip()
    x_4 = x_4.strip('[]')
    x_4 = x_4.split(", ")
    lists_4.append(x_4)
a_4 = np.array(lists_4)
a_4 = a_4.astype(float)
print (a_4)
file_4.close()
print(a_4.shape)

plt.plot(a_1,label='oringal mobilenet')
plt.plot(a_2,label='depthwise conv')
plt.plot(a_3,label='shuffle g=4')
plt.plot(a_4,label='shuffle g=1')
plt.xlabel('interactions(x2500)')
plt.ylabel('mAP')
plt.legend()
plt.savefig("result.jpg", dpi=100)
plt.show()
