#include "cost.h"

int main() {
    int paramnumber = 4;
    double *current = new double[paramnumber];
    current[0] = -340;
    current[1] = -330;
    current[2] = -320;
    current[3] = -350;

    Cost *cost = new Cost;
    cost->Init();
    double rss_1 = cost->Value(current);

    current[0] = -340;
    current[1] = -330;
    current[2] = -320;
    current[3] = 0;

    double rss_2 = cost->Value(current);
    // TODO: Calculate criterion for F test
    return 1;
}
    