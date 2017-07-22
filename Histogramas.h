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
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <functional>
#include <random>

#ifndef HISTOGRAMAS_H
#define HISTOGRAMAS_H

using namespace std;

class Histograma {
public:
    vector<float> vbin;
    vector<float> vcont;
    int num_clases;

    vector<float> r;

    void HistoVal(vector<float>, float, int);

    vector<float> Clases();
    
    vector<float> Conteos();

    void Ver();

private:
    int cont;
    float sumA;
    float sA;
    float bin_num;
    float bin;
    float bin0;
};

#endif /* HISTOGRAMAS_H */

