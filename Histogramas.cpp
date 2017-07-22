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
#include "Histogramas.h"

using namespace std;

void Histograma::HistoVal(vector<float> rx, float valmax, int numclases) {
    sA = valmax; //93.2036
    num_clases = numclases; //64
    r = rx;
}

vector<float> Histograma::Clases() {
    vbin.resize(num_clases);
    fill(vbin.begin(),vbin.end(),0);
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
    fill(vcont.begin(),vcont.end(),0);
    sumA = 0;
    for (int i = 1; i < num_clases; i++) {
        cont = 1;
        for (int j = 1; j < r.size(); j++) {
            if (vbin[i-1] < r[j] && r[j] < vbin[i]) {
                cont++;
                sumA++;
            }
        }
        vcont[i] = cont;
    }
    
    for(int i=0; i<num_clases; i++){
        vcont[i]=vcont[i]/sumA;
    }
    return vcont;
}

void Histograma::Ver() {
    for (unsigned int i = 0; i < num_clases - 1; i++) {
        cout << round(vbin[i]) << "-" << round(vbin[i]) << ": ";
        cout << string(500 * vcont[i], 'o') << endl;
    }
}