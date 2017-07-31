/*ESPAÑOL
 * El programa esta hecho para analizar secuencias de imagenes de patrones de gotas. 
 * En el programa principal se solicita al usuario intorducir el número de imagen de una 
 * imagen de referencia de la secuencia de imágenes de patrones de gotas. A partir de esta imagen se le
 * obtienen valores geométricos de las gotas, para lo cual se utilizan las funciones:
 * 
 * -Binarizar,
 * -Radios.
 * 
 * además, se produce un histograma que clasifica el conteo de gotas. Estos histogramas se pueden
 * mostrar ya sea, como caracteres en la salida en consola del programa y una imagen con la 
 * clase:
 * 
 * Histograma.
 * 
 * y las funciones:
 * 
 * -HistoVal,
 * -Clases,
 * -Conteos.
 * 
 * y las funciones para visualizar:
 * 
 * -Ver,
 * -imHistograma.
 * 
 * Además se pueden obtener valores básicos estádisticos de los descriptores de 
 * geométricos de gotas con las siguientes funciones:
 * 
 * -Maximo,
 * -Mínimo,
 * -Promedio,
 * -Media,
 * -DesvEst,
 * -Varianza.
 * 
 * A partir de los valores de clase y del conteo de gotas se calculan los coeficientes de la
 * ecuación discreta de Smoluchowski a tercer orden. Para ello, primero se computa un 
 * valor hipótesis de radio de cluster y una forma determinada del Kernel de la unión de dos 
 * o mas gotas con la clase:
 * 
 * -SMKernel.
 * 
 * y con las funciones:
 * 
 * -K_p,
 * -r_s.
 * 
 *  el cálculo de coeficientes se realiza con la clase
 * 
 * -SM.
 * 
 * y las funciones:
 * 
 * -PSM012
 * coef 1: PSM0,
 * coef 2: PSM1,
 * coef 3: PSM2.
 *
 *   
 * A partir de los coeficientes se puede proponer un nuevo histograma de gotas que se
 * aproxime al histograma de una imagen posterior a la imagen de referencia. Para ello,
 * la forma del kernel es muy importante.
 * 
 * ENGLISH
 * The following program attempts to analyze a sequence of images from droplet pattern.
 * The main program requests to a user to introduce the reference number of a photo from the
 * sequence. The following functions binaryze the image and extract geometrical features, 
 * in this case, the droplet radius. 
 *
 * -Binarizar,
 * -Radios.
 * 
 * Moreover, the program computes statistical values, and obtain a histogram using 
 * the following classe:
 * 
 * -Histograma.
 * 
 * and functions:
 * 
 * -HistoVal,
 * -Clases,
 * -Conteos. 
 * 
 * furthermore, basic statistical values can be extracted with fuctions:
 * 
 * -Maximo,
 * -Minimo,
 * -Promedio,
 * -Media,
 * -DesvEst,
 * -Varianza.
 * 
 * The program uses the bin vector and count vector from the histograms to calculate 
 * until third order coeficients of Smoluchowski's equation expansion. This is performed by 
 * setting a hypothetical radius value and checking if the next image one fits with the new 
 * histogram computed by Smoluchowski's model. The objects for this calculation are defined 
 * by the Kernel class:
 * 
 * -SMKernel.
 * 
 * along with the functions:
 * 
 * -K_p,
 * -r_s.
 * 
 *  and for the estimation of coeficients we use:
 * 
 * -SM.
 * 
 * along with the functions:
 * 
 * -PSM012
 * coef 1: PSM0,
 * coef 2: PSM1,
 * coef 3: PSM2.
 * 
 * From those coeficients we could expand the differential equation's solution for time as a
 * polynomium of third order.
 * 
 * May be it's complicated for those who don't speak Spanish find that the code 
 * has comments in this language. However, I try to don't use slang or long sentences 
 * in Spanish in case you want to use the Google translator service.
 * 
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

    /*Define el nombre de la imagen de referencia*/
    string fotoref = "ito2-";
    string extjpg = ".jpg";
    string exttxt = ".txt";
    string fotopost = "ito2-";
    cout << "indica foto de referencia: " << endl;
    int entr;
    cin>> entr;

    string numref = to_string(entr);
    string arxivref = fotoref + numref + extjpg;
    string arxivdatosref = "datos_ref-" + numref + exttxt;

    /*Abre la imagen e inicia el proceso de binarizacion*/
    float periMax, periMin;
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

    /*Funcion para encontrar radios de gotas*/
    Radios(contornos_im, radios);

    /* Crea el objeto opara operar histogramas y definir un vector de clase y otro de
    conteo*/
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

    /*Salida en consola con la forma del histograma*/
    histo1.Ver();

    /*Parametros y datos necesarios para el computo de la ecuacion de SM*/
    /*Distribucion para el radio de referencia de gota*/
    vector<float> hist2_r(hist1_cont.size());
    vector<float> hist2_cont(hist1_cont.size());
    float r_ref;

    /*Introduccion de la hipotesis de radio de cluster para el calculo de coef. de
     SM.*/

    cout << "introduzca valor de radio de referencia: " << endl;
    cin >>r_ref;

    int l;
    l = 0;

    fill(hist2_r.begin(), hist2_r.end(), 0);
    fill(hist2_cont.begin(), hist2_cont.end(), 0);

    hist1_r[0] = 0.0001;
    hist2_r[0] = 0.0001;

    while (hist1_r[l] <= r_ref) {
        l++;
        hist2_cont[l] = hist1_cont[l];
    }


    for (int i = 0; i < hist1_r.size(); i++) {
        hist2_r[i] = abs(hist1_r[i] - hist1_r[l] + 0.035);
    }

    cout << endl;

    /*
     * Se propne dos funcion Kernel para las clases de la muestra y 
     * para la hipotesis.
     * 
     */
    
    SMKernel ker1, ker2;
    vector<float> xk1(hist1_r.size());
    vector<float> xk2(hist1_r.size());
    
    /*Se proponen potencias y coeficientes  para el Kernel de SM */
    
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
    cout << "coeficientes del polinomio \n";
    cout << coef0 << endl;
    cout << coef1 << endl;
    cout << coef2 << endl;

    return 0;
}
