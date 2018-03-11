#include <cmath>
#include <iostream>

void TDMA(double *a, double *b, double *c, double *rhs, double *old, int Nx) {
    double *beta = new double[Nx];
    double *y = new double[Nx];
    beta[0] = c[0] / b[0];
    for(int nx=1; nx<Nx-1; nx++) {
        beta[nx] = c[nx] / (b[nx] - a[nx]*beta[nx-1]);
    }
    y[0] = rhs[0] / b[0];
    for(int nx=1; nx<Nx; nx++) {
        y[nx] = (rhs[nx] - a[nx]*y[nx-1]) / (b[nx] - a[nx]*beta[nx-1]);
    }
    old[Nx-1] = y[Nx-1];
    for(int nx=Nx-2; nx>=0; nx--) {
        old[nx] = y[nx] - beta[nx]*old[nx+1];
    }
    delete []beta;
    delete []y;
}

void interdcoeff(double *vars, double *d, double *conc, int n) {
    double T = 1000.0;
    for(int nx=0; nx<n; nx++) {
        double c = conc[nx];
        d[nx] = (1.0-c)*exp((c*vars[0]*1000.0 + (1.0-c)*vars[1]*1000.0)/8.31451/T) + c*exp((c*vars[2]*1000.0 + (1.0-c)*vars[3]*1000.0)/8.31451/T);;
    }
}

void simulate(double *vars, double *conc, int Nx) {
    double dx = 1.0e-6;
    double atime = 36000;
    double *interd = new double[Nx];
    double *a = new double[Nx];
    double *b = new double[Nx];
    double *c = new double[Nx];
    double *rhs = new double[Nx];

    for(int nx=0; nx<Nx/2; nx++) {
        conc[nx] = 0.2;
    }
    for(int nx=Nx/2; nx<Nx; nx++) {
        conc[nx] = 0.3;
    }

    double dt = 100;
    double elasped = 0.0;
    double h = dx;
    while(elasped <= atime) {
        interdcoeff(vars, interd, conc, Nx);
        for(int nx=1; nx<Nx-1; nx++) {
            double Dn = interd[nx];
            double Dnm = interd[nx-1];
            double Dnp = interd[nx+1];
            a[nx] = - 1.0/(h + h)*(Dnm + Dn)/h;
            b[nx] = 1.0/ dt + ((Dnp + Dn)/h + (Dnm + Dn)/h) / (h + h);
            c[nx] = -1.0/(h + h)*(Dnp + Dn)/h; 
            rhs[nx] = 1.0 / dt * conc[nx];
        }
        b[0] = 1.0; c[0] = -1.0; rhs[0] = 0.0;
        b[Nx-1] = 1.0; a[Nx-1] = -1.0; rhs[Nx-1] = 0.0;
        TDMA(a, b, c, rhs, conc, Nx);

        elasped += dt;
    }
}
