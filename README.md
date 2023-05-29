# algorithm_ws
|Ubuntu|ROS.ver|Compiler|Cpu|Gpu|Memory|Device|
|------|---|---|---|---|---|---|
|18.04|Melodic|c++11|i9-9980HK|GeForce GTX-1650|32G|Dell-XPS-9570|
|||c++14|Apple Silicon M1||16G|2021MAC-Pro 14|
---
## CMAKE_build
### Run command ###
```
cd ~/algorithm_ws/cmake_build
mkdir build
cd build
cmake .. && make
```
0. [virtual_class](https://github.com/ytam1208/algorithm_ws/blob/master/cmake_build/src/virtual_class.cpp)
1. [class_new](https://github.com/ytam1208/algorithm_ws/blob/master/cmake_build/src/class_new.cpp)
2. [inline](https://github.com/ytam1208/algorithm_ws/blob/master/cmake_build/src/inline.cpp)
3. [shared_ptr](https://github.com/ytam1208/algorithm_ws/blob/master/cmake_build/src/shared_ptr.cpp)
4. [unique_ptr](https://github.com/ytam1208/algorithm_ws/blob/master/cmake_build/src/unique_ptr.cpp)
5. [using_function_pointer](https://github.com/ytam1208/algorithm_ws/blob/master/cmake_build/src/using_function_pointer.cpp)
6. [std::bind](https://github.com/ytam1208/algorithm_ws/blob/master/cmake_build/src/bind.cpp)
7. [std::function](https://github.com/ytam1208/algorithm_ws/blob/master/cmake_build/src/function.cpp)

---
## ROS_build 
### Algorithm 
* [Newton Rapshon Method](https://github.com/ytam1208/algorithm_ws/blob/master/README_dir/Newton_Rapshon_Method.md)

* [Newton Rapshon Code](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Algorithm/Algorithm_practice/src/Newton_Rapshon_Method.cpp)

* [Gauss Newton Method](https://github.com/ytam1208/algorithm_ws/blob/master/README_dir/Gauss-Newton_Method.md)

* [Gauss Newton Code](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Algorithm/Algorithm_practice/src/Gauss-Newton_Method.cpp)

### Stereo Vision
* [Disparity Map Method](https://opalescent-potato-6fd.notion.site/1-Stereo-Matching-1d3b920783f6471babbde3edcd2c70d9)
  
* [Disparity Map Code](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Opencv/src/Vision/Stereo/Get_disparity.cpp)
  
* [Stereo Image Stiching Method](https://opalescent-potato-6fd.notion.site/Image-Stiching-Panorama-566abf7c1049442795eb5fd55da847b0)
  
* [Stereo Image Stiching Code](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Opencv/src/Vision/Stereo/Stiching_image.cpp)


---
### Backjun
#### Basic Programing 
[1000, 1001, 1008, 1100, 1330, 2588, 2753, 2884, 9498, 10171, 10430, 10718, 10869, 14681, 2739, 10950, 8393, 15552, 2741, 2742, 11021, 11022, 2438, 2439, 10871, 2839]
#### Dynamic Programing 
[1463]
###### Study web https://www.acmicpc.net/problemset
---
### __Opencv__ [Computer vision(Algorithms)]
1. histogram
2. canny_edge
3. k-means
4. Otsu
5. RANSAC
6. calibration
7. harris_corner
8. Gaussian
9. meanshift
10. morphology
11. PerspetiveTopview
12. QRcode
13. [LSM](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Opencv/src/opencv_alg/Linear-LSM.cpp)
###### Reference Book(https://www.hanbit.co.kr/store/books/look.php?p_code=B4536726709)
---

### **C_function** [Study]
* [enum](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/enum.cpp)
* [virtual_function](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/virtual_function.cpp)
* [pthread](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/thread_ex.cpp)
* [templete](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/templete.cpp)
* [auto](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/auto.cpp)
* [break_event](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/break_event.cpp)
* [for_loop](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/for_loop.cpp)
* [function_pointer](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/function_pointer.cpp)
* [map](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/map.cpp)
* [multi_map](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/multi_map.cpp)
* [multi_thread](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/multi_thread.cpp)
* [multi_virtual_function](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/multi_virtual_function.cpp)
* [Overloading](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/Overloading.cpp)
* [thisClass](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/thisClass.cpp)
* [TryCatch](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/TryCatch.cpp)
* [typdef_function_pointer](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/typedef_pointer.cpp)
* [using](https://github.com/ytam1208/algorithm_ws/blob/master/ROS_build/src/Practice/C_function/src/using.cpp)
---
### Ros_Python [Basic Python Study]
0. ROS_msg [talker]
1. variable
2. String...[slide, replace, strip..]
---

