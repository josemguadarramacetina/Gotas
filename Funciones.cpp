/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <cstdlib>
#include <iostream>
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

#include "Funciones.h"

using namespace std;
using namespace cv;

Mat& Rojo(Mat &scr, Mat &dest) {

    assert(scr.type() == CV_8UC3);
    dest = Mat::zeros(scr.size(), CV_8UC3);
    Mat src1 = Mat::zeros(scr.size(), CV_8UC3);
    Mat src2 = Mat::zeros(scr.size(), CV_8UC3);

    inRange(scr, Scalar(0, 0, 0), Scalar(0, 0, 255), src1);

    inRange(scr, Scalar(0, 0, 0), Scalar(255, 0, 0), src2);

    dest = src1 - src2;

    return dest;
}

Mat& Binarizar(Mat &imagen, Mat &threshold_output_imagen) {

    Mat threshold_output_0_imagen = Mat::zeros(imagen.size(), CV_8UC1);
    Mat threshold_output_1_imagen = Mat::zeros(imagen.size(), CV_8UC1);
    threshold_output_imagen = Mat::zeros(imagen.size(), CV_8UC1);

    threshold(imagen, threshold_output_1_imagen, 0, 255, CV_THRESH_OTSU);
    adaptiveThreshold(imagen, threshold_output_0_imagen, 255,
            CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 101, 1);

    threshold_output_imagen = threshold_output_1_imagen + threshold_output_0_imagen;
    Mat im_floodfill_imagen = threshold_output_imagen.clone();
    floodFill(im_floodfill_imagen, cv::Point(0, 0), Scalar(255));
    bitwise_not(im_floodfill_imagen, threshold_output_imagen);

    return threshold_output_imagen;
}

vector<float>& Radios(vector<vector<Point>> &contours_frame1,
        vector<float> &radios_f1) {

    int contorno_frame1;
    contorno_frame1 = contours_frame1.size();
    vector<Moments> mu_frame1(contorno_frame1);
    vector<Point2f> mc_frame1(contorno_frame1);
    vector<Point2f> mc_2frame1(contorno_frame1);
    vector<float> mc2_frame1(contorno_frame1);
    vector<float> F1ejes1_diag(contorno_frame1);
    vector<float> F1ejes2_diag(contorno_frame1);
    vector<float> F1lat1(contorno_frame1);
    vector<float> F1lat2(contorno_frame1);
    vector<float> F1div_ejes(contorno_frame1);
    vector<float> indice_f1(contorno_frame1);
    vector<float> area_frame1(contorno_frame1);
    vector<RotatedRect> minRect_frame1(contorno_frame1);
    radios_f1.resize(contorno_frame1);
    std::vector<float> distancias_f1(contorno_frame1);


    float F1ejesX, F1ejesY, F1ejes, F1ejesX1, F1ejesY1, F1ejesX2,
            F1ejesY2, F1ejesX3, F1ejesY3;

    int mal, bien, l1;
    l1 = 0;
    mal = 0;
    bien = 0;

    for (int i = 1; i < contours_frame1.size(); i++) {

        mu_frame1[i] = moments(contours_frame1[i], true);
        minRect_frame1[i] = minAreaRect(Mat(contours_frame1[i]));

        F1ejesX = 0.0;
        F1ejesY = 0.0;
        F1ejesX1 = 0.0;
        F1ejesY1 = 0.0;
        F1ejesX2 = 0.0;
        F1ejesY2 = 0.0;
        F1ejesX3 = 0.0;
        F1ejesY3 = 0.0;

        Point2f rect_f1_points[4];

        minRect_frame1[i].points(rect_f1_points);

        F1ejesX = double(rect_f1_points[2].x - rect_f1_points[0].x); //diagonal 1
        F1ejesY = double(rect_f1_points[2].y - rect_f1_points[0].y);
        F1ejesX1 = double(rect_f1_points[3].x - rect_f1_points[1].x); //diagonal 2
        F1ejesY1 = double(rect_f1_points[3].y - rect_f1_points[1].y);
        F1ejesX2 = double(rect_f1_points[3].x - rect_f1_points[0].x); //lado 1
        F1ejesY2 = double(rect_f1_points[3].y - rect_f1_points[0].y);
        F1ejesX3 = double(rect_f1_points[3].x - rect_f1_points[2].x); //lado 2
        F1ejesY3 = double(rect_f1_points[3].y - rect_f1_points[2].y);

        F1lat1[i] = double(pow(pow(F1ejesX2, 2) + pow(F1ejesY2, 2), .5));
        F1lat2[i] = double(pow(pow(F1ejesX3, 2) + pow(F1ejesY3, 2), .5));

        if (F1lat2[i] == 0 && F1ejes1_diag[i] + F1ejes2_diag[i] != 0) {
            mal++;
            //            cout<<"n "<<i<<" todo mal"<<endl;
        } else {
            bien++;
            l1++;
            indice_f1[l1] = i;
            F1ejes1_diag[l1] = double(pow(pow(F1ejesX, 2) + pow(F1ejesY, 2), .5));
            F1ejes2_diag[l1] = double(pow(pow(F1ejesX1, 2) + pow(F1ejesY1, 2), .5));
            radios_f1[l1] = (F1ejes1_diag[l1] + F1ejes2_diag[l1]) / 4.0;
        }
    }
    return radios_f1;
}
