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
#include "Funciones.h"

using namespace std;
using namespace cv;
#define PI 3.14159265

RNG rng(12345);

template <typename T>

string to_string(T value) {
    ostringstream os;
    os << value;
    return os.str();
}

CvSeq* dps = 0;

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
    vector<Vec4i> jerarquia_im;

    Mat im_salida;

    Binarizar(imagen1, im_salida);

    findContours(im_salida, contornos_im, jerarquia_im,
            CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    /*Analisis de fotografia 1*/
    vector<float> radios(contornos_im.size());
    Radios(contornos_im, radios);
    //
    Histograma histo1;
    histo1.HistoVal(radios, 64, entr, "Agua");
    vector<float> hist1_r(64);
    vector<float> hist1_cont(64);
    hist1_r = histo1.Clases();
    hist1_cont = histo1.Conteos();
    Mat prueba;
    histo1.Promedio();
    histo1.Varianza();
    histo1.DesvEst();
    prueba = histo1.imHistograma();
    float promerio, desvs;

    
       histo1.Ver();

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
