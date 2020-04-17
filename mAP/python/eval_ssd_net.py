
'''
Detection with SSD
In this example, we will load a SSD model and use it to detect objects.
'''
from __future__ import print_function
import os
from os.path import join
import sys
import argparse
import cPickle as pickle
import numpy as np
import pandas as pd
from tqdm import tqdm
from PIL import Image, ImageDraw
# Make sure that caffe is on the python path:
#caffe_root = '/opt/caffe-ssd/caffe/'
caffe_root = '/opt/caffe-ssd/caffe/'
# os.chdir(caffe_root)
sys.path.insert(0, os.path.join(caffe_root, 'python'))
import caffe
import matplotlib.pyplot as plt

from google.protobuf import text_format
from caffe.proto import caffe_pb2

# sys.path.append('/train/execute/evalnet/util')
# import util

def get_labelmaplist(labelmap):
    num_labels = len(labelmap.item)
    labelmaplist = []
    for i in xrange(0, num_labels):
        labelmaplist.append(labelmap.item[i].display_name)
    return labelmaplist

def get_labelname(labelmap, labels):
    num_labels = len(labelmap.item)
    labelnames = []
    if type(labels) is not list:
        labels = [labels]
    for label in labels:
        found = False
        for i in xrange(0, num_labels):
            if label == labelmap.item[i].label:
                found = True
                labelnames.append(labelmap.item[i].display_name)
                break
        assert found == True
    return labelnames

class CaffeDetection:
    def __init__(self, gpu_id, model_def, model_weights, image_resize, labelmap_file):
        caffe.set_device(gpu_id)
        caffe.set_mode_gpu()
        
        self.image_resize = image_resize
        # Load the net in the test phase for inference, and configure input preprocessing.
        self.net = caffe.Net(model_def,      # defines the structure of the model
                             model_weights,  # contains the trained weights
                             caffe.TEST)     # use test mode (e.g., don't perform dropout)
         # input preprocessing: 'data' is the name of the input blob == net.inputs[0]
        self.transformer = caffe.io.Transformer({'data': self.net.blobs['data'].data.shape})
        self.transformer.set_transpose('data', (2, 0, 1))
        self.transformer.set_mean('data', np.array([104, 117, 123])) # mean pixel
        # the reference model operates on images in [0,255] range instead of [0,1]
        self.transformer.set_raw_scale('data', 255)
        # the reference model has channels in BGR order instead of RGB
        self.transformer.set_channel_swap('data', (2, 1, 0))

        # load PASCAL VOC labels
        file = open(labelmap_file, 'r')
        self.labelmap = caffe_pb2.LabelMap()
        text_format.Merge(str(file.read()), self.labelmap)

    def detect(self, image_file):
        '''
        SSD detection
        '''
        # set net to batch size of 1
        self.net.blobs['data'].reshape(1, 3, self.image_resize[1], self.image_resize[0])
        
#         pdb.set_trace()
        if args.lmdb is None:
            image = caffe.io.load_image(image_file)
        else:
            image = image_file

        #Run the net and examine the top_k results
        transformed_image = self.transformer.preprocess('data', image)
        self.net.blobs['data'].data[...] = transformed_image

        # Forward pass.
        detections = self.net.forward()['detection_out']

        # Parse the outputs.
        det_label = detections[0,0,:,1]
        det_conf = detections[0,0,:,2]
        det_xmin = detections[0,0,:,3]
        det_ymin = detections[0,0,:,4]
        det_xmax = detections[0,0,:,5]
        det_ymax = detections[0,0,:,6]
        det_label_list = det_label.tolist()
        det_labels = get_labelname(self.labelmap, det_label_list)

        result = []
        for i in xrange(det_conf.shape[0]):
            xmin = det_xmin[i] # xmin = int(round(det_xmin[i] * image.shape[1]))
            ymin = det_ymin[i] # ymin = int(round(det_ymin[i] * image.shape[0]))
            xmax = det_xmax[i] # xmax = int(round(det_xmax[i] * image.shape[1]))
            ymax = det_ymax[i] # ymax = int(round(det_ymax[i] * image.shape[0]))
            score = det_conf[i]
            label = int(det_label_list[i])
            label_name = det_labels[i]
            result.append([xmin, ymin, xmax, ymax, label, score, label_name])
        return result
    
        
def process(args):
    '''main '''
    ###START
#     evalfile = "/train/execute/Tiny/testlabeled.txt"
#     detailfile = "/train/execute/Tiny2/PR_detail.txt"
    outsavedir = args.output_dir
    
    detection = CaffeDetection(args.gpu,
                               args.prototxt, args.model,
                               args.size, args.labelmap)
    pred_conf_list = []
    image_label_list = []
    labelmaplist = get_labelmaplist(detection.labelmap)
    
    if args.lmdb is None:
        evalfile = args.image_list
        filelist = np.array(pd.read_csv(evalfile,sep='\n',header=None)).tolist()
        for i,item in enumerate(tqdm(filelist)):
            item_jpg = os.path.join(args.imgdir_add,item[0].split(' ')[0])
    #         print("dealing img:{}".format(item_jpg))
            result = detection.detect(item_jpg)
            image_label_list.append(item[0])
            pred_conf_list.append(result)
    else:
        import lmdb
        import cv2
        lmdb_env = lmdb.open(args.lmdb, readonly=True)
        with lmdb_env.begin() as lmdb_txn:
            lmdb_cursor = lmdb_txn.cursor()
            datum = caffe_pb2.AnnotatedDatum()
            innerdatum = caffe_pb2.Datum()
            
            for key, value in tqdm(lmdb_cursor):
                
                datum.ParseFromString(value)
#                 imgarray = np.fromstring(datum.datum.data, dtype=np.uint8)
#                 imgarray = imgarray.reshape(datum.datum.channels, datum.datum.height, datum.datum.width)
                data_array = np.asarray(bytearray(datum.datum.data), dtype="uint8")  
                img_array = cv2.imdecode(data_array, cv2.IMREAD_COLOR)
#                 pdb.set_trace()
#                 innerdatum.ParseFromString(datum.datum)
#                 data_array = caffe.io.datum_to_array(datum.datum)[0]
                
                img_array = img_array/255.0
                result = detection.detect(img_array)
                imageitem = key.lstrip("00000000_")
                image_label_list.append(imageitem)
                pred_conf_list.append(result)
#         lmdb_env.close()
    
    
    if not os.path.exists(outsavedir):
        os.mkdir(outsavedir)
    with open(outsavedir + '/ssdpredfile.npz', 'wb') as scoreoutfile:
        pickle.dump(labelmaplist, scoreoutfile, True)
        pickle.dump(image_label_list, scoreoutfile, True)
        pickle.dump(pred_conf_list, scoreoutfile, True)
    print("data saved")
   

def parse_args():
    '''parse args'''
    parser = argparse.ArgumentParser(description = 'SSD model eval')
    parser.add_argument('-i', '--image_list', default='', type=str)
    parser.add_argument('-g', '--gpu', type=int, default=0)
    parser.add_argument('-b', '--basepath', default=None, type=str)
    parser.add_argument('-m', '--model', default=None, type=str)
    parser.add_argument('-p', '--prototxt', default=None, type=str)
    parser.add_argument('-o', '--output_dir', default=None, type=str)
    parser.add_argument('-l', '--labelmap',default=None, type=str)
    parser.add_argument('-s', '--size', default=None, type=str)
    parser.add_argument('-a', '--imgdir_add', default='', type=str)
    parser.add_argument('--lmdb', default=None, type=str)
    return parser.parse_args()

def init_param(args):
    if args.size is None:
        args.size = [1280,720]
    else:
        size = eval(args.size)
        args.size = size
    if args.basepath is not None:
        basepath = args.basepath
        if args.prototxt is not None:
            prototxt = join(basepath,args.prototxt)
            args.prototxt = prototxt
        if args.model is not None:
            model = join(basepath,args.model)
            args.model = model
        if args.image_list is not None:
            image_list = join(basepath,args.image_list)
            args.image_list = image_list
        if args.labelmap is not None:
            labelmap = join(basepath,args.labelmap)
            args.labelmap = labelmap
        if args.output_dir is None:
            output_dir = join(basepath,'ssdpred_savefile')
            args.output_dir = output_dir
    return args
            
        

if __name__ == '__main__':
    args = parse_args()
    args = init_param(args)
    print('Called with args:{}'.format(args))
    process(args)
    print('all done')
    