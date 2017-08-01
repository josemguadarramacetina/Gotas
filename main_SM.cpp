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
 * the following class:
 * 
 * -Histograma.
 * 
 * and functions:
 * 
 * -HistoVal,
 * -Clases,
 * -Conteos. 
 * 
 * furthermore, basic statistical values can be extracted with functions:
 * 
 * -Maximo,
 * -Minimo,
 * -Promedio,
 * -Media,
 * -DesvEst,
 * -Varianza.
 * 
 * The program uses the bin vector and count vector from the histograms to calculate 
 * until third order coefficients of Smoluchowski's equation expansion. This is performed by 
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
 *  and for the estimation of coefficients we use:
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
 * From those coefficients we could expand the differential equation solution for time as a
 * polynomial of third order.
 * 
 * May be it's complicated for those who don't speak Spanish find that the code 
 * has comments in this language. However, I try to don't use slang or long sentences 
 * in Spanish in case you want to use the Google translator service.
 * 
 * File:   main_SM.cpp
 * Author: Jose Guadarrama-Cetina
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
#include <unistd.h>
#include <dirent.h>

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
    string Directorio;
    /*
    cout<<"introduce la ubicacion de las imagenes"<<endl;
    cin>> Directorio;
    cin.get();
     */
    Directorio = "/home/jose/Documents/experimento_emulsion_BF/23022011/jpgs2";

    chdir(Directorio.c_str());

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
    string arxiv_agua_ref = "azul" + arxivref;

    /*Se obtiene el número de archivos totales que tienen el patrón del 
     * prefijo de la imagen:
     */

    DIR *dir;
    dir = opendir(".");
    struct dirent *dp;
    char *arx;
    int s;
    s = 0;

    while ((dp = readdir(dir)) != NULL) {
        arx = dp->d_name;
        if (strstr(arx, "ito2-")) {
            s++; //numero total de imagenes "ito2-"
        }
    }

    /*Abre la imagen e inicia el proceso de binarizacion*/
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

    /*Analisis de fotografia 1:
     * Funcion para encontrar radios de gotas: se inicializa vector y se aplica la
     * funcion.
     */
    vector<float> radios(contornos_im.size());
    Radios(contornos_im, radios);

    /* Se crea una imagen en el canal Blue con los contornos encontrados para comparar 
     * con una imagen posterior en la secuencia de imagenes
     */

    Mat azul = Mat::zeros(imagen1.size(), CV_8UC3);
    for (int i = 1; i != contornos_im.size(); i++) {
        drawContours(azul, contornos_im, i, Scalar(255, 0, 0), CV_FILLED);
    }
    //imwrite(arxiv_agua_ref, agua_referencia); //(opcional).

    /* Crea el objeto opara operar histogramas 
     * y definir un vector de clase y otro de conteo
     */
    Histograma histo1;
    histo1.HistoVal(radios, 64, entr, "Agua");
    vector<float> hist1_r(64);
    vector<float> hist1_cont(64);
    hist1_r = histo1.Clases();
    hist1_cont = histo1.Conteos();
    histo1.Promedio();
    histo1.Varianza();
    histo1.DesvEst();
    Mat prueba = histo1.imHistograma();
    /*Salida en consola con la forma del histograma*/
    //    histo1.Ver();

    /*entrada de los valores de la imagen de posterior a la referencia*/
    cout << "indica foto a comparar con la foto de referencia: " << endl;
    cout << " de preferencia que el numero sea diferente a " << entr << endl;
    cin>> entr;

    numref = to_string(entr);
    string arxivpost = fotoref + numref + extjpg;
    string arxivdatospost = "datos_ref-" + numref + exttxt;
    string arxiv_post = "rojo" + arxivpost;


    /*Abre la imagen e inicia el proceso de binarizacion*/
    //cargar imagen
    Mat imagen2 = imread(arxivpost, 1);
    cvtColor(imagen2, imagen2, CV_BGR2GRAY);

    //inicializa vectores para propiedades de contorno
    vector<vector<Point> > contornos_im2;
    vector<Vec4i> jerarquia_im2;

    Mat im_salida2;
    Binarizar(imagen2, im_salida2);
    findContours(im_salida2, contornos_im2, jerarquia_im2,
            CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    /*Analisis de fotografia 1:
     * Funcion para encontrar radios de gotas: se inicializa vector y se aplica la
     * funcion.
     */

    vector<float> radios2(contornos_im2.size());
    Radios(contornos_im2, radios2);

    Histograma histo2;
    histo2.HistoVal(radios2, 64, entr, "Mezcla");
    vector<float> hist2_r(64);
    vector<float> hist2_cont(64);
    hist2_r = histo2.Clases();
    hist2_cont = histo2.Conteos();
    histo2.Promedio();
    histo2.Varianza();
    histo2.DesvEst();
    Mat prueba2 = histo2.imHistograma();

    /* Se crea una imagen en el canal Blue con los contornos encontrados para comparar 
     * con una imagen posterior en la secuencia de imagenes
     */
    
    Mat roja = Mat::zeros(imagen2.size(), CV_8UC3);
    for (int i = 1; i != contornos_im2.size(); i++) {
        drawContours(roja, contornos_im2, i, Scalar(0, 0, 255), CV_FILLED);
    }
    //imwrite(arxiv_post, agua_post); //(opcional).
    /*Se combinan las imagenes azul y roja en proporciones iguales*/

    Mat combinacion = Mat::zeros(imagen2.size(), CV_8UC3);
    addWeighted(azul, 1.0, roja, 1.0, 0.0, combinacion);

    Mat resultante = Mat::zeros(imagen2.size(), CV_8UC3);


    Rojo(combinacion, resultante);
    /*La imagen "resultante se vuelve a analizar*/
    vector<vector<Point> > contornos_res;
    vector<Vec4i> jerarquia_res;
    findContours(resultante, contornos_res, jerarquia_res,
            CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<float> radios3(contornos_res.size());
    Radios(contornos_res, radios3);

    Histograma histo3;
    histo3.HistoVal(radios3, 64, entr, "Res");
    vector<float> hist3_r(64);
    vector<float> hist3_cont(64);
    hist3_r = histo3.Clases();
    hist3_cont = histo3.Conteos();
    histo3.Promedio();
    histo3.Varianza();
    histo3.DesvEst();
    Mat prueba3 = histo3.imHistograma();


    /*Aqui termina el analisis básico de las imágenes, ahora sigue la aplicacion 
     del modelo.
     */
    /*Parametros y datos necesarios para el computo de la ecuacion de SM*/
    /*Distribucion para el radio de referencia de gota*/
    vector<float> histr(hist1_cont.size());
    vector<float> histcont(hist1_cont.size());
    float rref;

    /*Introduccion de la hipotesis de radio de cluster para el calculo de coef. de
     SM.*/

    cout << "introduzca valor de radio de referencia: " << endl;
    cin >>rref;

    int l;
    l = 0;

    fill(histr.begin(), histr.end(), 0);
    fill(histcont.begin(), histcont.end(), 0);

    hist1_r[0] = 0.0001;
    histr[0] = 0.0001;

    while (hist1_r[l] <= rref) {
        l++;
        histcont[l] = hist1_cont[l];
    }
    
    for (int i = 0; i < hist1_r.size(); i++) {
        histr[i] = abs(hist1_r[i] - hist1_r[l] + 0.035);
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
    ker2.K_p(hist1_r, histr, 3.0, 0.6, -1);
    xk1 = ker1.r_s();
    xk2 = ker2.r_s();

    float coef0;
    float coef1;
    float coef2;

    SM funcion;
    funcion.PSM012(hist1_cont, histcont, xk1, xk2, l, 64);

    coef0 = funcion.PMS0();
    coef1 = funcion.PMS1();
    coef2 = funcion.PMS2();

    cout << "coeficientes del polinomio \n";
    
    cout << coef0 << endl;
    cout << coef1 << endl;
    cout << coef2 << endl;


    return 0;
}
