/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Funciones.h
 * Author: jose
 *
 * Created on July 24, 2017, 1:46 PM
 */
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"


#ifndef FUNCIONES_H
#define FUNCIONES_H
#pragma once

using namespace std;
using namespace cv;

Mat& Rojo(Mat& src, Mat &dest);

Mat& Binarizar(Mat &imagen, Mat &threshold_output_imagen);

vector<float>& Radios(vector<vector<Point>> &contours_frame1,
        vector<float> &radios_f1);


#endif /* FUNCIONES_H */

