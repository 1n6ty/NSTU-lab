/**
 * @file    estimate.h
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 *          Header file for estimate class.
 */

#pragma once

#include "../empirical/empirical.h"
#include "../core/interface.h"

class Estimate : public IObserver {
private:
    Empirical &e;
    double sigma;
    double c;
    int p;
    double mu;

    double rho(double x) const;

    void estimate();
public:
    Estimate(Empirical& e0, double sigma0,  double c0, int p0=100);
    ~Estimate(){e.detach(this);}

    double weight(double x);
    void update(){estimate();}

    double getMu() const {return this->mu;};

    void setSigma(double new_sigma);
    void setC(double new_c);
    void setP(int new_p);
};
