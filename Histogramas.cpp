/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <functional>
#include <random>
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
#include "Histogramas.h"
#include "Funciones.h"

using namespace std;
using namespace cv;

void Histograma::HistoVal(vector<float> rx, int numclases, 
        int numfoto, string nom) {
    sA = 93.2036;
    num_clases = numclases; //64
    r = rx;
    rMax = 0.0;
    sum = 0.0;
    rMin = 99999999.99;
    E = 0;
    nombre = nom;
    foto = numfoto;
    extensionjp=".jpg";
}

vector<float> Histograma::Clases() {

    vbin.resize(num_clases);
    fill(vbin.begin(), vbin.end(), 0);
    sumA = 0;
    bin = 0.001;
    for (int i = 0; i < num_clases; ++i) {
        cont = 1;
        bin = (sA / num_clases)*(float) i;
        vbin[i] = bin;
    }
    return vbin;
}

vector<float> Histograma::Conteos() {
    vcont.resize(num_clases);
    fill(vcont.begin(), vcont.end(), 0);
    sumA = 0;
    for (int i = 1; i < num_clases; i++) {
        cont = 1;
        for (int j = 1; j < r.size(); j++) {
            if (vbin[i - 1] < r[j] && r[j] < vbin[i]) {
                cont++;
                sumA++;
            }
        }
        vcont[i] = cont;
    }

    for (int i = 0; i < num_clases; i++) {
        vcont[i] = vcont[i] / sumA;
    }
    return vcont;
}

void Histograma::Ver() {
    for (unsigned int i = 0; i < num_clases - 1; i++) {
        cout << round(vbin[i]) << "-" << round(vbin[i]) << ": ";
        cout << string(500 * vcont[i], 'o') << endl;
    }
}

Mat Histograma::imHistograma() {
    RNG rng(12345);
    salida_1 = Mat::zeros(1536, 2048, CV_8UC3);
    salida = salida_1;
    bb = 93.2036;
    final_histo = 0;
    while (final_histo < 4) {
        bin_num = 4.0;
        min_bin = final_histo;
        bin = 0;
        bin0 = 0;
        num_bin = num_clases;
        bin_img = 1900 / num_bin;

        bin_hist = to_string(bin_num + min_bin) + "-";
        arxivhistoagua = "histo_agua-" + to_string(foto)
                + "-" + bin_hist + extensionjp;

        salida_1 = Mat::zeros(1536, 2048, CV_8UC3);

        for (int i = 1; i < num_clases; i++) {

            bin = vbin[i];
            bin0 = vbin[i - 1];

            color = Scalar(rng.uniform(255, 255), rng.uniform(0, 0),
                    rng.uniform(0, 0));
            line(salida_1, Point(bin_img * i + 72, 8), Point(bin_img * i + 72, 1400),
                    CV_RGB(128, 255, 100), 1.5, 4);
            putText(salida_1, nombre, Point(1350, 60), FONT_HERSHEY_PLAIN, 3.65,
                    CV_RGB(255, 255, 255), 3, true);
            putText(salida_1, "#", Point(20, 1370), FONT_HERSHEY_PLAIN, 2.65,
                    CV_RGB(255, 255, 255), 3, true);
            putText(salida_1, "t = " + to_string(foto), Point(1350, 120),
                    FONT_HERSHEY_PLAIN, 3.65, CV_RGB(255, 255, 255), 3, true);
            putText(salida_1, "Promedio = " + to_string(round(Prom * 100) / 100),
                    Point(1350, 180), FONT_HERSHEY_PLAIN, 3.65,
                    CV_RGB(255, 255, 255), 3, true);
            putText(salida_1, "Desv std = " + to_string(round(desvstd * 100) / 100),
                    Point(1350, 240), FONT_HERSHEY_PLAIN, 3.65,
                    CV_RGB(255, 255, 255), 3, true);
            putText(salida_1, " r ", Point(1350, 240), FONT_HERSHEY_PLAIN, 3.65,
                    CV_RGB(255, 255, 255), 3, true);

        }

        for (int i = 0; i < num_clases; i+=5) {
            putText(salida_1, to_string(round(
                    (min_bin + (bb / num_clases) * i) * 10) / 10),
                    Point(bin_img * i + 74, 1480), FONT_HERSHEY_PLAIN, 1.65,
                    CV_RGB(255, 255, 255), 3, true);
        }
        line(salida_1, Point(0, 1400), Point(2048, 1400), CV_RGB(128, 255, 100)
                , 1.5, CV_AA);
        maxHS0 = 0;
        for (int i = 1; i < num_clases; i++) {
            if (maxHS0 < vcont[i]) {
                maxHS0 = vcont[i];
            }
        }
        if (final_histo == 0) {
            maxHS = maxHS0;
        }

        for (int i = 1; i < 24; i += 5) {
            putText(salida_1, to_string((24 - i) * maxHS / 24), Point(6, float(1329 / 24) * i),
                    FONT_HERSHEY_PLAIN, 1.5, CV_RGB(255, 255, 255), 3, true);
        }
        limite2 = 1400;
        limite1 = 0;
        for (int i = 1; i < num_clases; i++) {
            if (vcont[i] > 0) {
                limite1 = limite2 - (vcont[i] )*1356;
                rectangle(salida_1, Point(round(bin_img * i + 74), limite2),
                        Point(round(bin_img * i + 100), limite1), CV_RGB(180, 250, 250)
                        , -2.5, 8);
            }
        }
        final_histo = bin;
        imwrite(arxivhistoagua, salida_1);
        salida = salida_1;
        salida_1.release();
    }

    return salida;
}

float Histograma::Maximo() {
    for (int i = 0; i < r.size(); i++) {
        if (rMax < r[i]) {
            rMax = r[i];
        }
    }

    return rMax;
}

float Histograma::Minimo() {
    for (int i = 0; i < r.size(); i++) {
        if (rMin < r[i]) {

            rMin = r[i];
        }
    }

    return rMin;
}

float Histograma::Promedio() {
    for (int i = 0; i < r.size(); i++) {
        sum += r[i];
    }
    Prom = sum / r.size();
    return Prom;
}

float Histograma::Varianza() {
    for (int i = 0; i < r.size(); i++) {
        E += pow((r[i] - Prom), 2);
    }
    vari = E / r.size();
    return vari;
}

float Histograma::DesvEst() {
    desvstd = sqrt(vari);
    return desvstd;
}

float Histograma::Media() {

    a = rMax;
    b = rMin;
    return (a - b) / 2;
}


//Mat& HistogramasPic(vector<float> &radios, float &promedio,
//        float &desvstd, int &num_bin, string &numref0, string &extjpg0) {
//    float bb, bin, bin0, bin_img;
//    float bin_num;
//    int final_histo, min_bin, binl2;
//    bb = 93.2036;
//    final_histo = 0;
//
//    while (final_histo < 4) {
//
//        bin_num = 4.0; // valor cercano a 10/64.
//        min_bin = final_histo; //(48/24)*2+95.833;
//
//        bin = 0;
//        bin0 = 0;
//
//        //        num_bin = 64; // no confundir con bin_num
//        bin_img = 1900 / num_bin;
//        binl2 = 0;
//
//        string bin_hist = to_string(bin_num + min_bin);
//        string arxivhistoagua = "histo_agua-" + numref0 + "-" + bin_hist
//                + extjpg0;
//
//        vector<Point2f> Lista(num_bin);
//        Mat histograma = Mat::zeros(2048, 1536, CV_8UC3);
//
//        for (int i = 1; i < num_bin; i++) {
//            bin = (bb / num_bin)*(i);
//            bin0 = (bb / num_bin)*(i - 1);
//            binl2 = 0;
//
//            for (int i1 = 1; i1 < radios.size(); i1++) {
//                if (radios[i1] > bin0 && radios[i1] < bin) {
//                    binl2++;
//                }
//            }
//
//            Lista[i] = Point2f(bin / 2, binl2);
//
//            binl2 = 0;
//            Scalar color = Scalar(rng.uniform(255, 255), rng.uniform(0, 0), rng.uniform(0, 0));
//            line(histograma, Point(bin_img * i + 72, 8), Point(bin_img * i + 72, 1400),
//                    CV_RGB(128, 255, 100), 1.5, 4);
//            putText(histograma, "Agua", Point(1400, 60), FONT_HERSHEY_PLAIN, 3.65,
//                    CV_RGB(255, 255, 255), 3, true);
//            putText(histograma, "n", Point(20, 1370), FONT_HERSHEY_PLAIN, 2.65,
//                    CV_RGB(255, 255, 255), 3, true);
//            putText(histograma, "t = " + numref0, Point(1400, 120), FONT_HERSHEY_PLAIN, 3.65,
//                    CV_RGB(255, 255, 255), 3, true);
//            putText(histograma, "Promedio = " + to_string(round(promedio * 100) / 100), Point(1400, 180), FONT_HERSHEY_PLAIN, 3.65,
//                    CV_RGB(255, 255, 255), 3, true);
//            putText(histograma, "Desv std = " + to_string(round(desvstd * 1000) / 1000), Point(1400, 240), FONT_HERSHEY_PLAIN, 3.65,
//                    CV_RGB(255, 255, 255), 3, true);
//            putText(histograma, " r ", Point(1980, 1485), FONT_HERSHEY_PLAIN, 2.65,
//                    CV_RGB(255, 255, 255), 3, true);
//        }
//
//        for (int i = 0; i < num_bin; i += 10) {
//            putText(histograma, to_string(round((min_bin + (bb / num_bin)*(i))*10) / 10),
//                    Point(bin_img * (i) + 74, 1480),
//                    FONT_HERSHEY_PLAIN, 1.65,
//                    CV_RGB(255, 255, 255), 3, true);
//        }
//
//        line(histograma, Point(0, 1400), Point(2048, 1400), CV_RGB(128, 255, 100), 1.5, CV_AA);
//        int maxHS0, maxHS;
//        maxHS0 = 0;
//
//        for (int i = 1; i < num_bin; i++) {
//            if (maxHS0 < Lista.at(i).y) {
//                maxHS0 = Lista.at(i).y;
//            }
//        }
//
//        if (final_histo == 0) {
//            maxHS = maxHS0;
//        }
//
//        for (int i = 1; i < 24; i += 5) {
//            putText(histograma, to_string((24 - i) * maxHS / 24), Point(6, float(1329 / 24) * i),
//                    FONT_HERSHEY_PLAIN, 1.5, CV_RGB(255, 255, 255), 3, true);
//            //    	putText(histograma,"-",Point(18,float(1329/24)*i),FONT_HERSHEY_PLAIN,2,
//            //                	CV_RGB(255,255,255),3,true);
//        }
//
//        float limite1, limite2;
//        limite2 = 1400.0;
//        limite1 = 0.0;
//        for (int i = 1; i < num_bin; i++) {
//            if (Lista.at(i).y > 0) {
//                limite1 = limite2 - (Lista.at(i).y / maxHS)*1356;
//                cv::Point pt1(bin_img * (i) + 74, limite1);
//                cv::Point pt2(bin_img * (i) + 100, limite2);
//                rectangle(histograma, pt1, pt2,
//                        CV_RGB(0, 150, 255), -2.5, 8);
//            }
//        }
//
//        final_histo = bin;
//        cout << "final histo " << final_histo << endl;
//        Lista.clear();
//        imwrite(arxivhistoagua, histograma);
//        histograma.release();
//
//    }
//    
//}

