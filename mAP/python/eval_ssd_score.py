from __future__ import print_function
import xml.etree.ElementTree as ET
import pickle
import argparse
import numpy as np
import matplotlib.pyplot as plt
import os
from tqdm import tqdm
from PIL import Image,ImageDraw

def load_data(ssddatafile):
    with open(ssddatafile,'rb') as ssddata:
        read_labelmap_list = pickle.load(ssddata)
        print("read_labelmap_list loaded")
        read_image_label = pickle.load(ssddata)
        print("image_label loaded")
        read_pred_conf = pickle.load(ssddata)
        print("pred data loaded")
    return read_labelmap_list, read_image_label, read_pred_conf

def get_label(read_labelmap_list, read_image_label):
    labels = []
    add_dir = args.xmldir_add
    for i,img_label in enumerate(read_image_label):
        if 2==len(img_label.split(' ')):
            img,xml = [os.path.join(add_dir,x) for x in img_label.split(' ')]
        else:
            img = os.path.join(add_dir,img_label)
            xml = img.replace('.jpg','.xml')
        tree = ET.parse(xml)
        root = tree.getroot()
        tinylist = []
        for subitem in root.findall('size'):
            width = float(subitem.find('width').text)
            height = float(subitem.find('height').text)
        for subitem in root.findall('object'):
            name = subitem.find('name').text
            for items in subitem.findall('bndbox'):
                xmin = (float(items.find('xmin').text)/width)
                ymin = (float(items.find('ymin').text)/height)
                xmax = (float(items.find('xmax').text)/width)
                ymax = (float(items.find('ymax').text)/height)
            for i in range(len(read_labelmap_list)):
                if read_labelmap_list[i] == name:
                    label = i
                    break
                    
            sublist = [xmin, ymin, xmax, ymax, label, name]
            tinylist.append(sublist)
        labels.append(tinylist)
    # print(labels)
    return labels
    print("all done")

def drawimg(img_label,pred):
    add_dir = args.xmldir_add
    if 2==len(img_label[0].split(' ')):
        img,xml = [os.path.join(add_dir,x) for x in img_label[0].split(' ')]
    else:
        img = os.path.join(add_dir,img_label[0])
    img_base = os.path.basename(img)
    savedir = os.path.join(args.output_dir,'predimg')
    if not os.path.exists(savedir):
        os.mkdir(savedir)
    saveimg = os.path.join(args.output_dir,'predimg',img_base)
    IMG = Image.open(img)
    size = IMG.size
    draw = ImageDraw.Draw(IMG)
    for xmin,ymin,xmax,ymax,label_p in pred:
        coodi = [xmin*size[0],ymin*size[1],xmax*size[0],ymax*size[1]]
        draw.rectangle(coodi,outline=(255,0,0))
        draw.text((xmin*size[0],ymin*size[1]),str(label_p),fill=(0,255,255))
    IMG.save(saveimg,'JPEG')

def ssd_analyse(read_image_label,read_pred_conf,labels,label_num):
    IOU_THRED = args.Iou_thread
    
    ssd_evalout_detail_file = args.output_dir + '/ssd_evalout_detail.txt'
    evallist = []
    thredllist = [float(x)/100.0 for x in range(1,10)]
    thredmlist = [float(x+1)/10.0 for x in range(9)]
    thredhlist = [float(x)/100.0 for x in range(91,100)]
    thredlist = thredllist+thredmlist+thredhlist
    print("data analysing....")
    with open(ssd_evalout_detail_file,'w') as df:
        for thred in tqdm(thredlist):
            misslabel = [0 for _ in range(label_num)]
            matchnum = [0 for _ in range(label_num)]
            resipred = [0 for _ in range(label_num)]
            loc_loss = [0 for _ in range(label_num)]
            iou_avg = [0 for _ in range(label_num)]
            detailfile = os.path.join(args.output_dir,'preddraw_th_{}.txt'.format(thred))
            with open(detailfile,'w') as wf:
                for i,img_label in enumerate(read_image_label):
                    img_pred_list = []
                    pred = np.array(read_pred_conf[i])
                    labels_oneimg = labels[i]
                    if pred.shape == (0,):
                        for i in range(label_num):
                            misnum = sum(np.array(labels_oneimg)[:,4].astype(np.float32)==(i+1))
                            misslabel[i] += misnum
                        continue
                    choose_id = np.array(map(float,pred[:,5]))>thred
                    pred_filtered = np.array(pred[choose_id,:5],dtype=np.float32)
                    if args.preddraw:
#                         img_pred_list.append(img_label)
                        print([img_label,pred_filtered.tolist()],file=wf)
                    tiny_filttered = []
                    prednum = []
                    for i in range(label_num):
                        tiny_id = np.array(map(float,pred_filtered[:,4]))==(i+1)
                        tiny_filttered.append(pred_filtered[tiny_id,:])
                        prednum.append(sum(tiny_id))
                    pred_name = pred[choose_id,6]
                    for tiny_item in labels_oneimg:
                        xmin,ymin,xmax,ymax,labelitem,name = tiny_item
                        labelitem -= 1
                        if(tiny_filttered[labelitem].shape[0]):
                            loc = np.argmin(np.sum(map(abs,tiny_filttered[labelitem]-tiny_item[:5]),axis=1))
                            xmin_p,ymin_p,xmax_p,ymax_p,label_p = tiny_filttered[labelitem][loc]
                            xmi = max(xmin_p,xmin)
                            ymi = max(ymin_p,ymin)
                            xma = min(xmax_p,xmax)
                            yma = min(ymax_p,ymax)
                            if xmi >= xma or ymi >= yma:
            #                     print("loc miss match")
                                misslabel[labelitem] += 1
                                continue
                            Iou_xl = (xma-xmi)/(xmax-xmin)
                            Iou_xp = (xma-xmi)/(xmax_p-xmin_p)
                            Iou_yl = (yma-ymi)/(ymax-ymin)
                            Iou_yp = (yma-ymi)/(ymax_p-ymin_p)
                            Iou_al = Iou_xl*Iou_yl
                            Iou_ap = Iou_xp*Iou_yp
                            if (Iou_al+Iou_ap) < IOU_THRED*2:
            #                     print("Iou miss match")
                                misslabel[labelitem] += 1
                                continue
            #                 print("IOU between pred and label is:{} and {}".format(Iou_ap,Iou_al))
                            matchnum[labelitem] += 1
                            iou_avg[labelitem] += (Iou_al+Iou_ap)/2
                            loc_loss[labelitem] += np.min(np.sum(map(abs,tiny_filttered[labelitem]-tiny_item[:5]),axis=1))
                            tiny_filttered[labelitem] = np.delete(tiny_filttered[labelitem], loc, axis=0)  
                            prednum[labelitem] -= 1
                        else:
                            misslabel[labelitem] += 1
                    for i in range(label_num):
                        resipred[i] += prednum[i]

            thredlist = []
            misslabel_s = sum(misslabel)
            matchnum_s = sum(matchnum)
            resipred_s = sum(resipred)
            if matchnum_s!= 0:
                iou_avg_s = sum(iou_avg)/matchnum_s
            if matchnum_s == 0:
                print("\nNO MATCH OBJECT!! THRED:{}\tmatchnum:{}\tmisslabel:{}\tresipred:{}".format(thred,matchnum_s,misslabel_s,resipred_s),file=df)
            else:
                acc_s = round(float(matchnum_s)/float(matchnum_s+misslabel_s+resipred_s),3)
                precision_s = round(float(matchnum_s)/float(matchnum_s+resipred_s),3)
                recall_s = round(float(matchnum_s)/float(matchnum_s+misslabel_s),3)
                thredlist.extend([thred,acc_s,precision_s,recall_s])
                print("\nTHRED:{}\tACC:{}\tPRECISION:{}\tRECALL:{}\tmatchnum:{}\tmisslabel:{}\tresipred:{}\tiou_avg:{}".format(thred,acc_s,precision_s,recall_s,matchnum_s,misslabel_s,resipred_s,iou_avg_s),file=df)
            for i in range(label_num):
                if matchnum[i]==0:
                    acc = 0
                    precision = 0
                    recall = 0
                else:
                    acc = round(float(matchnum[i])/float(matchnum[i]+misslabel[i]+resipred[i]),3)
                    precision = round(float(matchnum[i])/float(matchnum[i]+resipred[i]),3)
                    recall = round(float(matchnum[i])/float(matchnum[i]+misslabel[i]),3)
                thredlist.extend([i,acc,precision,recall])
                if matchnum[i] == 0:
                    print("LABEL:{} \tmatchnum:{}\tmisslabel:{}\tresipred:{}".format(i,matchnum[i],misslabel[i],resipred[i]),file=df)
                else:
                    print("LABEL:{} \tACC:{}\tPRECISION:{}\tRECALL:{}\tmatchnum:{}\tmisslabel:{}\tresipred:{}\tiou_avg:{}".format(i,acc,precision,recall,matchnum[i],misslabel[i],resipred[i],iou_avg[i]/matchnum[i]),file=df)
            evallist.append(thredlist)
            if 0.5 == thred:
                default_th_pr = thredlist
    print("object matching done")
    print("PR curve creating")
    for i in tqdm(range(label_num+1)):
        fig = plt.figure(figsize=(8,8))
        plt.axis([0,1,0,1])
        plt.ylabel('precision')
        plt.xlabel('recall')
        plt.xticks(np.arange(0, 1, 0.1))
        plt.yticks(np.arange(0, 1, 0.1))
        if 0 == i:
            plt.title('ensemble PRC')
            saveimg = args.output_dir + '/Ssd_EvalPR_Ensembel.png'
        else:
            plt.title('Label {} PRC'.format(i))
            saveimg = args.output_dir + '/Ssd_EvalPR_Label_{}.png'.format(i)
        plt.grid(True)
        evalarr = np.array(evallist)
        plt.plot(evalarr[:,4*i+3], evalarr[:,4*i+2], 'b')
        plt.plot(evalarr[:,4*i+3], evalarr[:,4*i+2], 'o')
        plt.plot(default_th_pr[4*i+3], default_th_pr[4*i+2], '*')
        plt.savefig(saveimg)
    print("PR curve saved")

def parse_args():
    '''parse args'''
    parser = argparse.ArgumentParser(description = 'SSD model scores')
    parser.add_argument('-f', '--eval_file', default='', type=str)
    parser.add_argument('-o', '--output_dir', default=None, type=str)
    parser.add_argument('-a', '--xmldir_add', default='', type=str)
    parser.add_argument('-t', '--Iou_thread', default=0.3, type=float)
    parser.add_argument('-p', '--preddraw', default=True, type=bool)
    return parser.parse_args()

def init_param(args):
    if args.output_dir is None:
        args.output_dir = os.path.dirname(args.eval_file)
    return args

if __name__ == '__main__':
    args = parse_args()
    args = init_param(args)
    print('Called with args:{}'.format(args))
    read_labelmap_list, read_image_label, read_pred_conf = load_data(args.eval_file)
    label_num = len(read_labelmap_list)-1
    labels = get_label(read_labelmap_list, read_image_label)
    ssd_analyse(read_image_label,read_pred_conf,labels,label_num)
    print('all done')
    