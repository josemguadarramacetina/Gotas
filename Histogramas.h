/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Histogramas.h
 * Author: jose
 *
 * Created on July 21, 2017, 11:27 AM
 */
#ifndef HISTOGRAMAS_H
#define HISTOGRAMAS_H

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
#pragma once

using namespace std;
using namespace cv;

class Histograma {
public:
    vector<float> vbin;
    vector<float> vcont;
    vector<float> r;
    
    Scalar color;

    int num_clases;
    int foto;
    float rMax;
    float rMin;
    float Prom;
    float Med;
    float desvstd;
    float vari;
    float ave;
    int num_bin;
    int bin_img;


    void HistoVal(vector<float>, int, int, string);/* vector de datos, clases, numero de imagen, extension JPG, nombre de la muestra*/
    vector<float> Clases();/*Crea un vector de clases*/
    vector<float> Conteos();/* Cuenta las muestras segun la clase */
    void Ver();

    Mat imHistograma();/*Funcion que crea histogramas en formato JPG*/

    float Maximo();
    float Minimo();
    float Promedio();
    float Media();/*Max-Min/2*/
    float DesvEst();/*La desviacion estandar se obtiene de la varianza*/
    float Varianza();/* Calcula la varianza*/

private:
    Mat salida;
    Mat salida_1;
    int cont;
    float sumA;
    float sA;
    float bin_num;
    float bin;
    float bin0;
    float bb;
    int final_histo;
    float maxHS0;
    float maxHS;
    float min_bin;
    float limite1;
    float limite2;


    float sum = 0;
    float E;
    float a;
    float b;

    string extensionjp;
    string nombre;
    string bin_hist;
    string arxivhistoagua;

};


#endif /* HISTOGRAMAS_H */

