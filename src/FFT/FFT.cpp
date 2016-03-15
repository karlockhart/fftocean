/*

Project: Ocean
Author: DEISS Olivier

This software is offered under the GPL license. See COPYING for more information.

*/

#include <cmath>
#include <iostream>
#include <vector>

#include "FFT.hpp"

/* FFT constructor */
FFT::FFT(int p_n, std::vector<double> p_real, std::vector<double> p_imag) :
    n(p_n),
    p(0),
    real(p_real),
    imag(p_imag) {
    while(p_n!=1) {
        p_n/=2;
        p++;
    }
}

/* FFT */
void FFT::radix_direct() {
    int                 n_copy = n;
    std::vector<double> real_copy; real_copy.resize(n);
    std::vector<double> imag_copy; imag_copy.resize(n);
    for(int i=0 ; i<p ; i++) {                   // processus à faire _p fois
        for(int j=0 ; j<n_copy/2 ; j++) {              // calculer pour n/2 sous parties
            for(int k=0 ; k<pow(2, i) ; k++) {    // calcul
                int    index1     = k+j*pow(2, i+1);
                int    index2     = index1 + pow(2, i);
                double var        = static_cast<double>(-(2*M_PI)/pow(2, i+1))*index1;
                double v_cos      = cos(var);
                double v_sin      = sin(var);
                double imag2      = imag[index2];
                double real2      = real[index2];
                double real1      = real[index1];
                double imag1      = imag[index1];
                real_copy[index1] = real1 + v_cos*real2 - v_sin*imag2;
                real_copy[index2] = real1 - v_cos*real2 + v_sin*imag2;
                imag_copy[index1] = imag1 + v_cos*imag2 + v_sin*real2;
                imag_copy[index2] = imag1 - v_cos*imag2 - v_sin*real2;
            }
        }
        swap(real_copy, real);
        swap(imag_copy, imag);
        n_copy /= 2;
    }
}

/* FFT-1 */
void FFT::radix_reverse() {
    int                 n_copy = n;
    std::vector<double> real_copy; real_copy.resize(n);
    std::vector<double> imag_copy; imag_copy.resize(n);
    for(int i=0 ; i<p ; i++) {                    // processus à faire _p fois
        for(int j=0 ; j<n_copy/2 ; j++) {         // calculer pour n/2 sous parties
            for(int k=0 ; k<pow(2, i) ; k++) {    // calcul
                int    index1     = k+j*pow(2, i+1);
                int    index2     = index1 + pow(2, i);
                double var        = static_cast<double>((2*M_PI)/pow(2, i+1))*index1;
                double v_cos      = cos(var);
                double v_sin      = sin(var);
                double imag2      = imag[index2];
                double real2      = real[index2];
                double real1      = real[index1];
                double imag1      = imag[index1];
                real_copy[index1] = real1 + v_cos*real2 - v_sin*imag2;
                real_copy[index2] = real1 - v_cos*real2 + v_sin*imag2;
                imag_copy[index1] = imag1 + v_cos*imag2 + v_sin*real2;
                imag_copy[index2] = imag1 - v_cos*imag2 - v_sin*real2;
            }
        }
        swap(real_copy, real);
        swap(imag_copy, imag);
        n_copy /= 2;
    }
}

/* Sorts the data before applying the radix algorithm */
void FFT::sort() {
    int n_copy = n;
    for(int i=0 ; i<p-1 ; i++) {        // processus à répéter _p - 1 fois
        std::vector<double> sorted_R; sorted_R.reserve(n);
        std::vector<double> sorted_I; sorted_I.reserve(n);
        std::vector<double> vectRp;   vectRp.resize(n_copy/2);
        std::vector<double> vectIp;   vectIp.resize(n_copy/2);
        std::vector<double> vectRi;   vectRi.resize(n_copy/2);
        std::vector<double> vectIi;   vectIi.resize(n_copy/2);
        std::vector<double>::iterator itR(sorted_R.begin());
        std::vector<double>::iterator itI(sorted_I.begin());
        for(int j=0 ; j<n/n_copy ; j++) {       // réorganiser -n/n sous parties
            for(int k=0 ; k<n_copy/2 ; k++) {   // réorganisation
                double index = 2*k+j*n_copy;
                vectRp[k] = real[index];
                vectIp[k] = imag[index];
                vectRi[k] = real[index+1];
                vectIi[k] = imag[index+1];
            }
            sorted_R.insert(itR, vectRp.begin(), vectRp.end());
            sorted_I.insert(itI, vectIp.begin(), vectIp.end());
            sorted_R.insert(sorted_R.end(), vectRi.begin(), vectRi.end());
            sorted_I.insert(sorted_I.end(), vectIi.begin(), vectIi.end());
            itR = sorted_R.end();
            itI = sorted_I.end();
        }
        swap(sorted_R, real);
        swap(sorted_I, imag);
        n_copy /= 2;
    }
}
