/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cstdlib>
#include <iostream>
#include <vector>
#include <math.h>
#include "SMKernel.h"

using namespace std;

void SMKernel::K_p(vector<float> r1, vector<float> r2, float coef1, float pot1, float pot2) {
    a = r1;
    b = r2;
    c = coef1;
    p1 = pot1;
    p2 = pot2;
}

vector<float> SMKernel::r_s() {
    cr.resize(a.size());
    for (int i = 1; i < a.size(); i++) {
        cr[i] = c * (pow(a[i], p1) + pow(b[i], p1))*
                (pow(a[i], p2) + pow(b[i], p2));
    }
    return cr;
}

