#include <stdlib.h> 
#include <chrono>
#include <random>
#include <fstream>
#include "ficklaw.h"
#include <fstream>

double uniform() {
    return double(rand()) / RAND_MAX;
}

struct Cost {
    double *ob;
    double *pr;
    double *pstar;
    double sigmastar;
    int Nx;
    void Init();
    double Value(double *p);
};

void Cost::Init() {
    Nx = 100;
    ob = new double[Nx];
    pr = new double[Nx];

    pstar = new double[4];
    pstar[0] = -340;
    pstar[1] = -330;
    pstar[2] = -320;
    pstar[3] = -350;

    simulate(pstar, ob, Nx);
    sigmastar = 0.01;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    std::normal_distribution<double> distribution (0.0, sigmastar);

    for(int nx=0; nx<Nx; nx++) {
        ob[nx] += distribution(generator);
    }

    //save the observation to file for visualization
    std::ofstream outbuf("ob.bin", std::ios::binary);
    outbuf.write(reinterpret_cast<char*>(&Nx), sizeof(int));
    outbuf.write(reinterpret_cast<char*>(&ob[0]), sizeof(double)*Nx);
    outbuf.close();
}

double Cost::Value(double *p) {
    simulate(p, pr, Nx);

    double chisq = 0.0;
    for(int nx=0; nx<Nx; nx++) {
        chisq += (pr[nx] - ob[nx])*(pr[nx] - ob[nx]);
    }

    return chisq / Nx;
}