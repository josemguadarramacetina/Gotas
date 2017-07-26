/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SMKernel.h
 * Author: jose
 *
 * Created on July 21, 2017, 10:56 AM
 */
#include <cstdlib>
#include <iostream>
#include <vector>

#ifndef SMKERNEL_H
#define SMKERNEL_H

using namespace std;

class SMKernel {
public:
    vector<float> a;
    vector<float> b;
    float c;
    float p1;
    float p2;
    void K_p(vector<float>, vector<float>, float, float, float);

    vector<float> r_s();

private:
    vector<float> cr;

};


#endif /* SMKERNEL_H */

