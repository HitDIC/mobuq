#include "cost.h"

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

    std::cout << "Beginning of MCMC simulation...\n" << uniform() << " " << uniform();

    double alpha = 0.1;

    double *current = new double[paramnumber];
    for(int i=0; i<paramnumber; ++i) {
        current[i] = lowerlimit[i] + uniform()*(upperlimit[i] - lowerlimit[i]);
    }
    double procurrent = Probability(current);

    std::cout << "pcurrent: " << procurrent << std::endl;

    double *tmp = new double[paramnumber];

    std::cout << "Before the loop...\n";

    std::ofstream outbuf("seq.txt", std::ios::out);

    for(int i=0; i<4000; i++) {
        
        // Random-walk Metropolis algorithm
        for(int i=0; i<paramnumber; ++i) {
            tmp[i] = current[i] + alpha*(uniform()-0.5)*(upperlimit[i] - lowerlimit[i]);
        }

        double p = Probability(tmp);
        double rc = std::min(p / procurrent, 1.0);

        double r = uniform();
        if(r < rc) {
            // accept the estimation
            for(int i=0; i<paramnumber; ++i) {
                current[i] = tmp[i];
            }
            procurrent = p;
        }

        for(int i=0; i<paramnumber; ++i) {
            current[i] = tmp[i];
        }

        // restore the current value to the file
        for(int i=0; i<paramnumber; ++i) {
            outbuf << current[i] << " ";
        }
        outbuf << procurrent << "\n";
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
        mc->upperlimit[i] = -300;
    }

    std::cout << "Before evolving...\n";
    mc->Evolve();
    std::cout << "End of MCMC simulation...\n";
    return 1;
}