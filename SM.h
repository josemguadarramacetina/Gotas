/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SM.h
 * Author: jose
 *
 * Created on July 21, 2017, 3:43 PM
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

#ifndef SM_H
#define SM_H

using namespace std;

class SM {
public:

    vector<float> dist;
    vector<float> distk;
    vector<float> xk0;
    vector<float> xk1;
    int obje;
    int clases;

    void PSM012(vector<float>, vector<float>,
            vector<float>, vector<float>, int, int ); 
    
    float PMS0();
    float PMS1();
    float PMS2();
 
private:
    vector<float> cc0_menos;
    vector<float> cc0_mas;
    vector<float> cc1_menos;
    vector<float> cc1_mas;
    vector<float> cc2_menos;
    vector<float> cc2_mas;
    float suma0;
    float suma1;
    float suma2;
    float suma0_menos;
    float suma1_menos;
    float suma2_menos;
    float suma0_mas;
    float suma1_mas;
    float suma2_mas;
    
};




#endif /* SM_H */

