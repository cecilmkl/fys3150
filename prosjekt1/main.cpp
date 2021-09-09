#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

double f(double x);
arma::vec general_algorithm(arma::vec a, arma::vec b, arma::vec c, arma::vec g, int npoints);

int main(int argc, const char * argv[]) {

    int i;
    int npoints = 10;
    arma::vec u = arma::vec(npoints);
    arma::vec x = arma::vec(npoints);
    double x_min = 0.0;
    double x_max = 1.0;
    double h = (x_max - x_min) / npoints;
    // Boundary conditions:
    double u_0 = 0; // u(0) = 0 // nye navn?
    double u_1 = 0;  // u(1)=0

    int width = 12;
    int prec = 4;

    //opening file
    ofstream ofile;
    ofile.open ("data.txt");

    //setting up the x-array and the solutions to the function u, and printing it to file
    for (i=0 ; i <= npoints ; i++){

        x(i) = h*i;
        u(i) = 1 - (1 - exp(-10)) * x(i) - exp(-10 * x(i));
        ofile << setw(width) << setprecision(prec) << scientific << x(i)
              << setw(width) << setprecision(prec) << scientific << u(i) << endl;

    }
    //close file
    ofile.close();


    // Problem 7
    // special case:
    arma::vec a = arma::vec(n).fill(-1.);
    arma::vec b = arma::vec(n).fill(2.);
    arma::vec c = arma::vec(n).fill(-1);

    // Making sure a and c only have n-1 values, but corresponding indices
    a(0) = 0.;
    c(npoints) = 0.;

    arma::vec v = arma::vec(n); // Declaring empty solution vector

    // Defining the g vector:
    arma::vec g = arma::vec(n);
    g(0) = h*h*100*exp(-10*x(0)) + u_0; // h^2*f1 + u(0)
    g(n-1) = h*h*100*exp(-10*x(n-1)) + u_1; // h^2*f1 + u(1)

    for (int i = 1; i <= n-2; i++){
      g(i) = h*h*100*exp(-10*x(i))
    }

    arma::vec v = general_algorithm(a,b,c,g,n);

    //opening file
    ofstream ofile;
    ofile.open ("approx_general.txt");

    //setting up the x-array and the solutions to the function u, and printing it to file
    for (i=0 ; i <= npoints-1 ; i++){
        ofile << setw(width) << setprecision(prec) << scientific << x(i)
              << setw(width) << setprecision(prec) << scientific << v(i) << endl;
    }
    //close file
    ofile.close();

    return 0;
}

arma::vec general_algorithm(arma::vec a, arma::vec b, arma::vec c, arma::vec g, int npoints){
    // check g!!!

    // Helpful new variables
    arma::vec btilde = arma::vec(n);
    arma::vec gtilde = arma::vec(n);
    btilde(0) = b(0);
    gtilde(0) = g(0);

    arma::vec v = arma::vec(n); // solution vector
    double tmp; // variable to reduce FLOPs

    for (int i = 1; i <= npoints -1; i++){ // n elements (n-1)?
      tmp = a(i) / b(i-1) * c(i-1);
      btilde(i) = b(i) - tmp;
      gtilde(i) = g(i) - tmp;
    }

    v(n-1) = gtilde(n-1) / btilde(n-1); // Last element can now be found directly

    for (int j = n-2; j >= 0; j--){ // n-1 elements
      v(j) = (gtilde(j) - (c(j) * v(j+1))) / btilde(j);
    }
    return v;

}

double f(double x){
  return 100*exp(-10*x);
}
