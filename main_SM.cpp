/*El programa esta hecho para analizar secuencias de imagenes de patrones de gotas. La entrada principal del programa
 * es una primera imagen de referencia a la cual se le extraen propiedades de los valores de radio de gota y se calcula una
 * distribucion con un tamano de clase determinado. Posteriormente el programa calcula tambien una distribucion de radio de gota
 * en una imagen sucesiva y las compara mediante la ecuacion de coagulacion de Smoluchowski. El algoritmo de comparacion modifica la
 * forma de la funcion Kernel de tal forma que se localizan los parametros que describen la forma funcional por la cual cambian los
 * valores del radio de una distribucion a otra. 
 * 
 */

/* 
 * File:   main_SM.cpp
 * Author: jose
 *
 * Created on July 17, 2017, 6:01 PM
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
//#include "opencv2/contrib/contrib.hpp"
#include <vector>
#include <string>
#include <math.h>
#include <numeric>
#include <fstream>
#include <algorithm>
#include <functional>
#include <random>
#include "SMKernel.h"
#include "SM.h"
#include "Histogramas.h"

using namespace std;
using namespace cv;

RNG rng(12345);

template <typename T>

string to_string(T value) {
    ostringstream os;
    os << value;
    return os.str();
}


#define PI 3.14159265
CvSeq* dps = 0;

Mat Binarizar(Mat imagen) {

    Mat threshold_output_0_imagen = Mat::zeros(imagen.size(), CV_8UC1);
    Mat threshold_output_1_imagen = Mat::zeros(imagen.size(), CV_8UC1);
    Mat threshold_output_imagen = Mat::zeros(imagen.size(), CV_8UC1);

    threshold(imagen, threshold_output_1_imagen, 0, 255, CV_THRESH_OTSU);
    adaptiveThreshold(imagen, threshold_output_0_imagen, 255,
            CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 101, 1);

    threshold_output_imagen = threshold_output_1_imagen + threshold_output_0_imagen;
    Mat im_floodfill_imagen = threshold_output_imagen.clone();
    floodFill(im_floodfill_imagen, cv::Point(0, 0), Scalar(255));
    bitwise_not(im_floodfill_imagen, threshold_output_imagen);

    return threshold_output_imagen;
}

vector<float> Radios(vector<vector<Point>> contours_frame1) {
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
    std::vector<float> radios_f1(contorno_frame1);
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

/*funciones*/
float Maximo(vector<float> v) {
    float rMax = 0.0;
    for (int i = 0; i < v.size(); i++) {
        if (rMax < v[i]) {

            rMax = v[i];
        }
    }
    return rMax;
}

float Minimo(vector<float> v) {
    float rMin = 99999999.99;
    for (int i = 0; i < v.size(); i++) {
        if (rMin > v[i]) {

            rMin = v[i];
        }
    }
    return rMin;
}

float Promedio(vector<float> v) {
    float sum = 0;
    for (int i = 0; i < v.size(); i++)
        sum += v[i];

    return sum / v.size();
}

float Desviacion(vector<float> v, float ave) {
    float E = 0;
    for (int i = 0; i < v.size(); i++) {
        E += pow((v[i] - ave), 2);
    }
    return sqrt(E / v.size());
}

int main(int argc, char** argv) {

    string fotoref = "ito2-";
    string extjpg = ".jpg";
    string exttxt = ".txt";
    string fotopost = "ito2-";
    cout << "indica foto de referencia: " << endl;
    int entr;
    cin>> entr;
    float periMax, periMin;
    string numref = to_string(entr);
    string arxivref = fotoref + numref + extjpg;
    string arxivdatosref = "datos_ref-" + numref + exttxt;

    //cargar imagen
    Mat imagen1 = imread(arxivref, 1);
    cvtColor(imagen1, imagen1, CV_BGR2GRAY);
    //inicializa vectores para propiedades de contorno

    vector<vector<Point> > contornos_im;
    vector<Vec4i> gerarquia_im;

    Mat im_salida;

    im_salida = Binarizar(imagen1);

    findContours(im_salida, contornos_im, gerarquia_im,
            CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    /*Analisis de fotografia 1*/
    vector<float> radios(contornos_im.size());
    radios = Radios(contornos_im);
    //
    Histograma histo1, histo2;
    histo1.HistoVal(radios, 93.64, 64);
    vector<float> hist1_r(64);
    vector<float> hist1_cont(64);
    hist1_r = histo1.Clases();
    hist1_cont = histo1.Conteos();

    //    histo1.Ver();

    /*Parametros y datos necesarios para el computo de la ecuacion de SM*/
    /*Distribucion para el radio de referencia de gota*/
    vector<float> hist2_r(hist1_cont.size());
    vector<float> hist2_cont(hist1_cont.size());
    float r_ref;
    cout << "introduzca valor de radio de referencia: " << endl;
    cin >>r_ref;
    int l;
    l = 0;
    hist1_r[0] = 0.0001;
    hist2_r[0] = 0.0001;


    fill(hist2_r.begin(), hist2_r.end(), 0);
    fill(hist2_cont.begin(), hist2_cont.end(), 0);

    while (hist1_r[l] <= r_ref) {
        l++;
        hist2_cont[l] = hist1_cont[l];
    }

    for (int i = 0; i < hist1_r.size(); i++) {
        hist2_r[i] = abs(hist1_r[i] - hist1_r[l] + 0.035);
    }

    for (int i = 0; i < 64; i++) {
        cout << hist2_cont[i] << endl;
    }

    cout << endl;

    /*Se propne una funcion Kernel*/
    SMKernel ker1, ker2;
    vector<float> xk1(hist1_r.size());
    vector<float> xk2(hist1_r.size());
    ker1.K_p(hist1_r, hist1_r, 3.0, 0.6, -1);
    ker2.K_p(hist1_r, hist2_r, 3.0, 0.6, -1);
    xk1 = ker1.r_s();
    xk2 = ker2.r_s();
    
    float coef0;
    float coef1;
    float coef2;

    SM funcion;
    funcion.PSM012(hist1_cont, hist2_cont, xk1, xk2, l, 64);
    
    coef0 = funcion.PMS0();
    coef1 = funcion.PMS1();
    coef2 = funcion.PMS2();
    cout<<"coeficientes del polinomio \n";
    cout << coef0 << endl;
    cout << coef1 << endl;
    cout << coef2 << endl;
        
    return 0;
}

