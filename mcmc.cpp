#include <stdlib.h> 
#include <chrono>
#include <random>
#include "ficklaw.h"

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
}

double Cost::Value(double *p) {
    simulate(pstar, pr, Nx);

    double chisq = 0.0;
    for(int nx=0; nx<Nx; nx++) {
        chisq += (pr[nx] - ob[nx])*(pr[nx] - ob[nx]);
    }

    return chisq;
}

struct MCMC {
    double sigma;
    int paramnumber;
    double *lowerlimit, *upperlimit;
    Cost *cost;
    double Probability(double *p);
    void Evolve();
};

double MCMC::Probability(double *p) {
    double chisq = cost->Value(p) / 2.0 / sigma / sigma;
    return exp(-chisq);
}

void MCMC::Evolve() {

    std::cout << "Beginning of MCMC simulation...\n";

    double alpha = 0.01;

    double *current = new double[paramnumber];
    for(int i=0; i<paramnumber; ++i) {
        current[i] = lowerlimit[i] + uniform()*(upperlimit[i] - lowerlimit[i]);
    }
    double procurrent = Probability(current);

    double *tmp = new double[paramnumber];

    std::cout << "Before the loop...\n";

    while(true) {
        
        // Random-walk Metropolis algorithm
        for(int i=0; i<paramnumber; ++i) {
            tmp[i] = current[i] + alpha*uniform()*(upperlimit[i] - lowerlimit[i]);
        }

        double p = Probability(tmp);
        double rc = std::min(p / procurrent, 1.0);

        double r = uniform();
        std::cout << r << "\t" << rc << "\t" << p << std::endl;
        if(r < rc) {
            for(int i=0; i<paramnumber; ++i) {
                current[i] = tmp[i];
            }
        }
        // std::cout << cost->Value(current) << ", ";
        // for(int i=0; i<paramnumber; ++i) {
        //     std::cout << current[i] << ", ";
        // }
        // std::cout << std::endl;
    }

    delete []tmp;
    delete []current;
}

int main() {
    Cost *cost = new Cost;
    cost->Init();
    MCMC *mc = new MCMC;
    mc->cost = cost;
    mc->paramnumber = 4;
    mc->sigma = cost->sigmastar;
    mc->lowerlimit = new double[4];
    mc->upperlimit = new double[4];
    for(int i=0; i<4; i++) {
        mc->lowerlimit[i] = -400;
        mc->upperlimit[i] = 0;
    }

    std::cout << "Before evolving...\n";
    mc->Evolve();

    return 1;
}