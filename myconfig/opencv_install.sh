# install dependencies
cd ~/libs
unzip opencv-2.4.13.zip
cd opencv-2.4.13
mkdir release
cd release
mkdir installed

# sudo apt-get update
# sudo apt-get install -y build-essential
# sudo apt-get install -y cmake
# sudo apt-get install -y libgtk2.0-dev
# sudo apt-get install -y pkg-config
# sudo apt-get install -y python-numpy python-dev
# sudo apt-get install -y libavcodec-dev libavformat-dev libswscale-dev
# sudo apt-get install -y libjpeg-dev libpng-dev libtiff-dev libjasper-dev
#  
# sudo apt-get -qq install libopencv-dev build-essential checkinstall cmake pkg-config yasm libjpeg-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev python-dev python-numpy libtbb-dev libqt4-dev libgtk2.0-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils
 
# download opencv-2.4.11
# wget -c http://downloads.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.13/opencv-2.4.13.zip

# unzip opencv-2.4.13.zip
# cd opencv-2.4.13
# mkdir -p release
# cd release
 
# compile and install
# cmake -G "Unix Makefiles" -D CMAKE_CXX_COMPILER=/usr/bin/g++ CMAKE_C_COMPILER=/usr/bin/gcc -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/home/zhicheng/libs/opencv-2.4.13/release/installed BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_OPENGL=ON -D ENABLE_FAST_MATH=ON -D WITH_IMAGEIO=ON -D BUILD_SHARED_LIBS=OFF -D WITH_GSTREAMER=ON -D WITH_CUDA=ON -D CUDA_FAST_MATH=1 WITH_CUBLAS=1 ..
# make all -j20 # 24 cores
# sudo make install

# another compile
# cmake -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=/usr/bin/g++ CMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/home/zhicheng/libs/opencv-2.4.13/release/installed -DWITH_TBB=ON -DBUILD_NEW_PYTHON_SUPPORT=ON -DWITH_V4L=ON  -DBUILD_EXAMPLES=ON -DWITH_OPENGL=ON -DINSTALL_TO_MANGLED_PATHS=ON -DINSTALL_CREATE_DISTRIB=ON -DENABLE_FAST_MATH=ON -DWITH_IMAGEIO=ON -DWITH_GSTREAMER=ON -DBUILD_TIFF=ON ..

# another compile
cmake -G "Unix Makefiles" -DCMAKE_CXX_COMPILER=/usr/bin/g++ CMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/home/zhicheng/libs/opencv-2.4.13/release/installed -DINSTALL_TO_MANGLED_PATHS=ON -DINSTALL_CREATE_DISTRIB=ON  -DWITH_IMAGEIO=ON -DWITH_GSTREAMER=ON -DBUILD_TIFF=ON -DWITH_1394=OFF -DPYTHON_NUMPY_INCLUDE_DIR=/home/zhicheng/anaconda3/envs/caffe/lib/python2.7/site-packages/numpy/core/include ..

make all -j # 24 cores
sudo make install

# ignore libdc1394 error http://stackoverflow.com/questions/12689304/ctypes-error-libdc1394-error-failed-to-initialize-libdc1394
 
#python
#> import cv2
#> cv2.SIFT
#<built-in function SIFT>
