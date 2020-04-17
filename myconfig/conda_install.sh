conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge/

conda create --name caffe python=2.7

source activate caffe
conda install cython scipy scikit-image ipython h5py leveldb networkx nose python-dateutil protobuf python-gflags pyyaml pillow six
conda install -c menpo opencv 

source activate caffe

