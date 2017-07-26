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
#include "SM.h"

using namespace std;

void SM::PSM012(vector<float> distribucion, vector<float> distribucionk,
        vector<float> rkernel0, vector<float> rkernel1,
        int lj, int m) {
    dist = distribucion;
    distk = distribucionk;
    xk0 = rkernel0;
    xk1 = rkernel1;
    obje = lj;
    clases = m;
}

float SM::PMS0() {
    clases = dist.size();
    clases = dist.size();
    cc0_mas.resize(clases);
    cc0_menos.resize(clases);
    suma0_menos = 0;
    for (int i = 0; i < clases; i++) {
        cc0_mas[i] = 0.5 * xk0[i] * distk[i] * dist[obje];
        cc0_menos[i] = xk1[i] * dist[i] * dist[obje];
        suma0_menos += cc0_menos[i];
    }

    suma0_mas = 0;
    for (int i = 0; i < obje; i++) {
        suma0_mas += cc0_mas[i];
    }
    suma0 = suma0_mas - suma0_menos;
    return suma0;
}

float SM::PMS1() {
    clases = dist.size();
    clases = dist.size();
    cc1_mas.resize(clases);
    cc1_menos.resize(clases);
    suma1_menos = 0;
    for (int i = 0; i < clases; i++) {
        cc1_mas[i] = 0.5 * xk0[i] * (distk[i] * suma0*dist[obje] 
                + suma0*distk[i] * dist[obje]);
        cc1_menos[i] = xk1[i]*(dist[i] * suma0*distk[obje] 
                + suma0*dist[i] * dist[obje]);
        suma1_menos += cc1_menos[i];
    }

    suma1_mas = 0;
    for (int i = 0; i < obje; i++) {
        suma1_mas += cc1_mas[i];
    }
    suma1 = suma1_mas - suma1_menos;
    return suma1;
}

float SM::PMS2() {
    clases = dist.size();
    cc2_mas.resize(clases);
    cc2_menos.resize(clases);
    suma2_menos = 0;
    for (int i = 0; i < clases; i++) {
        cc2_mas[i] = 0.5 * xk0[i] *(distk[i] * suma1*dist[obje] + 
                suma0*dist[i] * suma0*distk[obje] 
                + suma1*distk[i] * dist[obje]);
        cc2_menos[i] = xk1[i] * (dist[i] * suma1*dist[obje] + 
                suma0*distk[i] * suma0*distk[obje] + 
                suma1*distk[obje] * dist[obje]);
        suma2_menos += cc2_menos[i];
    }

    suma2_mas = 0;
    for (int i = 0; i < obje; i++) {
        suma2_mas += cc2_mas[i];
    }
    suma2 = suma2_mas - suma2_menos;
    return suma2;
}