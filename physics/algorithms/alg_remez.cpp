/**
 * Copyright (c) 2013,2018 Alessandro Sciarra
 * Copyright (c) 2013 Matthias Bach
 * Copyright (c) 2014 Christopher Pinke
 *
 * This file is part of CL2QCD.
 *
 * CL2QCD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CL2QCD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CL2QCD. If not, see <http://www.gnu.org/licenses/>.
 *
 * NOTE: The code contained in this file was developed by external developers
 *       and the copyright and license statements above refer to the work
 *       that was done to include the third party code into CL2QCD.
 */

/** @file
 *
 * Copyright (c) Mike Clark - 25th May 2005
 *
 * alg_remez.C
 *
 * AlgRemez is an implementation of the Remez algorithm, which in this
 * case is used for generating the optimal nth root rational
 * approximation.
 *
 * Note this class requires the gnu multiprecision (GNU MP) library.
 *
 */

#include "alg_remez.hpp"

#include "../../host_functionality/logger.hpp"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Constructor
physics::algorithms::AlgRemez::AlgRemez(double lower, double upper, long precision)
{
    prec = precision;
    bigfloat::setDefaultPrecision(prec);

    apstrt = lower;
    apend  = upper;
    apwidt = apend - apstrt;

    if (logger.beDebug()) {
        logger.trace() << "Approximation bounds are [" << (double)apstrt << "," << (double)apend << "]";
        logger.trace() << "Precision of arithmetic is " << precision;
    }

    alloc = 0;
    n     = 0;
    d     = 0;

    foundRoots = 0;

    // Only require the approximation spread to be less than 1 ulp
    tolerance = 1e-15;
}

// Destructor
physics::algorithms::AlgRemez::~AlgRemez()
{
    if (alloc) {
        delete[] param;
        delete[] roots;
        delete[] poles;
        delete[] xx;
        delete[] mm;
        delete[] a_power;
        delete[] a;
    }
}

// Free memory and reallocate as necessary
void physics::algorithms::AlgRemez::allocate(int num_degree, int den_degree)
{
    // Arrays have previously been allocated, deallocate first, then allocate
    if (alloc) {
        delete[] param;
        delete[] roots;
        delete[] poles;
        delete[] xx;
        delete[] mm;
    }

    // Note use of new and delete in memory allocation - cannot run on qcdsp
    param = new bigfloat[num_degree + den_degree + 1];
    roots = new bigfloat[num_degree];
    poles = new bigfloat[den_degree];
    xx    = new bigfloat[num_degree + den_degree + 3];
    mm    = new bigfloat[num_degree + den_degree + 2];

    if (!alloc) {
        // The coefficients of the sum in the exponential
        a       = new bigfloat[SUM_MAX];
        a_power = new int[SUM_MAX];
    }

    alloc = 1;
}

// Reset the bounds of the approximation
void physics::algorithms::AlgRemez::setBounds(double lower, double upper)
{
    apstrt = lower;
    apend  = upper;
    apwidt = apend - apstrt;
}

// Generate the rational approximation x^(pnum/pden)
double physics::algorithms::AlgRemez::generateApprox(int degree, unsigned long pnum, unsigned long pden)
{
    return generateApprox(degree, degree, pnum, pden);
}

double
physics::algorithms::AlgRemez::generateApprox(int num_degree, int den_degree, unsigned long pnum, unsigned long pden)
{
    double* a_param = 0;
    int* a_pow      = 0;
    return generateApprox(num_degree, den_degree, pnum, pden, 0, a_param, a_pow);
}

// Generate the rational approximation x^(pnum/pden)
double physics::algorithms::AlgRemez::generateApprox(int num_degree, int den_degree, unsigned long pnum,
                                                     unsigned long pden, int a_len, double* a_param, int* a_pow)
{
    if (logger.beDebug()) {
        logger.trace() << "Degree of the approximation is (" << num_degree << "," << den_degree << ")";
        logger.trace() << "Approximating the function x^(" << pnum << "/" << pden << ")";
    }

    // Reallocate arrays, since degree has changed
    if (num_degree != n || den_degree != d)
        allocate(num_degree, den_degree);

    if (a_len > SUM_MAX) {
        logger.fatal() << "In AlgRemez::generateApprox. Error: a_length > SUM_MAX";
        exit(0);
    }

    step = new bigfloat[num_degree + den_degree + 2];

    a_length = a_len;
    for (int j = 0; j < a_len; j++) {
        a[j]       = a_param[j];
        a_power[j] = a_pow[j];
    }

    power_num = pnum;
    power_den = pden;
    spread    = 1.0e37;
    iter      = 0;

    n   = num_degree;
    d   = den_degree;
    neq = n + d + 1;

    initialGuess();
    stpini(step);

    while (spread > tolerance) {  // iterate until convergance

        if (iter++ % 100 == 0) {
            if (logger.beDebug())
                logger.trace() << "Iteration " << iter - 1 << ", spread " << (double)spread << " delta "
                               << (double)delta;
        }

        equations();
        if (delta < tolerance) {
            logger.fatal() << "In AlgRemez::generateApprox. Delta too small, try increasing precision";
            exit(0);
        }

        search(step);
    }

    int sign;
    double error = (double)getErr(mm[0], &sign);
    logger.info() << "Converged at " << iter << " iterations, error = " << std::setprecision(12) << error;

    // Once the approximation has been generated, calculate the roots
    if (!root()) {
        logger.error() << "In AlgRemez::generateApprox. Root finding failed!";
    } else {
        foundRoots = 1;
    }

    delete[] step;

    // Return the maximum error in the approximation
    return error;
}

// Return the partial fraction expansion of the approximation x^(pnum/pden)
int physics::algorithms::AlgRemez::getPFE(double* Res, double* Pole, double* Norm)
{
    if (n != d) {
        logger.fatal() << "In AlgRemez::getPFE. Cannot handle case: Numerator degree neq Denominator degree";
        return 0;
    }

    if (!alloc) {
        logger.fatal() << "In AlgRemez::getPFE. Approximation not yet generated";
        return 0;
    }

    if (!foundRoots) {
        logger.fatal() << "In AlgRemez::getPFE. Roots not found, so PFE cannot be taken";
        return 0;
    }

    bigfloat* r = new bigfloat[n];
    bigfloat* p = new bigfloat[d];

    for (int i = 0; i < n; i++)
        r[i] = roots[i];
    for (int i = 0; i < d; i++)
        p[i] = poles[i];

    // Perform a partial fraction expansion
    pfe(r, p, norm);

    // Convert to double and return
    *Norm = (double)norm;
    for (int i = 0; i < n; i++)
        Res[i] = (double)r[i];
    for (int i = 0; i < d; i++)
        Pole[i] = (double)p[i];

    delete[] r;
    delete[] p;

    // Where the smallest shift is located
    return 0;
}

// Return the partial fraction expansion of the approximation x^(-pnum/pden)
int physics::algorithms::AlgRemez::getIPFE(double* Res, double* Pole, double* Norm)
{
    if (n != d) {
        logger.fatal() << "In AlgRemez::getIPFE. Cannot handle case: Numerator degree neq Denominator degree";
        return 0;
    }

    if (!alloc) {
        logger.fatal() << "In AlgRemez::getIPFE. Approximation not yet generated";
        return 0;
    }

    if (!foundRoots) {
        logger.fatal() << "In AlgRemez::getIPFE. Roots not found, so PFE cannot be taken";
        return 0;
    }

    bigfloat* r = new bigfloat[d];
    bigfloat* p = new bigfloat[n];

    // Want the inverse function
    for (int i = 0; i < n; i++) {
        r[i] = poles[i];
        p[i] = roots[i];
    }

    // Perform a partial fraction expansion
    pfe(r, p, (bigfloat)1l / norm);

    // Convert to double and return
    *Norm = (double)((bigfloat)1l / (norm));
    for (int i = 0; i < n; i++) {
        Res[i]  = (double)r[i];
        Pole[i] = (double)p[i];
    }

    delete[] r;
    delete[] p;

    // Where the smallest shift is located
    return 0;
}

// Initial values of maximal and minimal errors
void physics::algorithms::AlgRemez::initialGuess()
{
    // Supply initial guesses for solution points
    long ncheb = neq;  // Degree of Chebyshev error estimate
    bigfloat a, r;

    // Find ncheb+1 extrema of Chebyshev polynomial

    a     = ncheb;
    mm[0] = apstrt;
    for (long i = 1; i < ncheb; i++) {
        r = 0.5 * (1 - cos((M_PI * i) / (double)a));
        // r *= sqrt_bf(r);
        r     = (exp((double)r) - 1.0) / (exp(1.0) - 1.0);
        mm[i] = apstrt + r * apwidt;
    }
    mm[ncheb] = apend;

    a = 2.0 * ncheb;
    for (long i = 0; i <= ncheb; i++) {
        r = 0.5 * (1 - cos(M_PI * (2 * i + 1) / (double)a));
        // r *= sqrt_bf(r); // Squeeze to low end of interval
        r     = (exp((double)r) - 1.0) / (exp(1.0) - 1.0);
        xx[i] = apstrt + r * apwidt;
    }
}

// Initialise step sizes
void physics::algorithms::AlgRemez::stpini(bigfloat* step)
{
    xx[neq + 1] = apend;
    delta       = 0.25;
    step[0]     = xx[0] - apstrt;
    for (int i = 1; i < neq; i++)
        step[i] = xx[i] - xx[i - 1];
    step[neq] = step[neq - 1];
}

// Search for error maxima and minima
void physics::algorithms::AlgRemez::search(bigfloat* step)
{
    bigfloat a, q, xm, ym, xn, yn, xx0, xx1;
    int i, meq, emsign, ensign, steps;

    meq          = neq + 1;
    bigfloat* yy = new bigfloat[meq];

    bigfloat eclose  = 1.0e30;
    bigfloat farther = 0l;

    xx0 = apstrt;

    for (i = 0; i < meq; i++) {
        steps = 0;
        xx1   = xx[i];  // Next zero
        if (i == meq - 1)
            xx1 = apend;
        xm = mm[i];
        ym = getErr(xm, &emsign);
        q  = step[i];
        xn = xm + q;
        if (xn < xx0 || xn >= xx1) {  // Cannot skip over adjacent boundaries
            q      = -q;
            xn     = xm;
            yn     = ym;
            ensign = emsign;
        } else {
            yn = getErr(xn, &ensign);
            if (yn < ym) {
                q      = -q;
                xn     = xm;
                yn     = ym;
                ensign = emsign;
            }
        }

        while (yn >= ym) {  // March until error becomes smaller.
            if (++steps > 10)
                break;
            ym     = yn;
            xm     = xn;
            emsign = ensign;
            a      = xm + q;
            if (a == xm || a <= xx0 || a >= xx1)
                break;  // Must not skip over the zeros either side.
            xn = a;
            yn = getErr(xn, &ensign);
        }

        mm[i] = xm;  // Position of maximum
        yy[i] = ym;  // Value of maximum

        if (eclose > ym)
            eclose = ym;
        if (farther < ym)
            farther = ym;

        xx0 = xx1;  // Walk to next zero.
    }               // end of search loop

    q = (farther - eclose);  // Decrease step size if error spread increased
    if (eclose != 0.0)
        q /= eclose;  // Relative error spread
    if (q >= spread)
        delta *= 0.5;  // Spread is increasing; decrease step size
    spread = q;

    for (i = 0; i < neq; i++) {
        q = yy[i + 1];
        if (q != 0.0)
            q = yy[i] / q - (bigfloat)1l;
        else
            q = 0.0625;
        if (q > (bigfloat)0.25)
            q = 0.25;
        q *= mm[i + 1] - mm[i];
        step[i] = q * delta;
    }
    step[neq] = step[neq - 1];

    for (i = 0; i < neq; i++) {  // Insert new locations for the zeros.
        xm = xx[i] - step[i];
        if (xm <= apstrt)
            continue;
        if (xm >= apend)
            continue;
        if (xm <= mm[i])
            xm = (bigfloat)0.5 * (mm[i] + xx[i]);
        if (xm >= mm[i + 1])
            xm = (bigfloat)0.5 * (mm[i + 1] + xx[i]);
        xx[i] = xm;
    }

    delete[] yy;
}

// Solve the equations
void physics::algorithms::AlgRemez::equations(void)
{
    bigfloat x, y, z;
    int i, j, ip;
    bigfloat* aa;

    bigfloat* AA = new bigfloat[(neq) * (neq)];
    bigfloat* BB = new bigfloat[neq];

    for (i = 0; i < neq; i++) {  // set up the equations for solution by simq()
        ip = neq * i;            // offset to 1st element of this row of matrix
        x  = xx[i];              // the guess for this row
        y  = func(x);            // right-hand-side vector

        z  = (bigfloat)1l;
        aa = AA + ip;
        for (j = 0; j <= n; j++) {
            *aa++ = z;
            z *= x;
        }

        z = (bigfloat)1l;
        for (j = 0; j < d; j++) {
            *aa++ = -y * z;
            z *= x;
        }
        BB[i] = y * z;  // Right hand side vector
    }

    // Solve the simultaneous linear equations.
    if (simq(AA, BB, param, neq)) {
        logger.fatal() << " In AlgRemez::equations. simq failed!";
        exit(0);
    }

    delete[] AA;
    delete[] BB;
}

// Evaluate the rational form P(x)/Q(x) using coefficients
// from the solution vector param
bigfloat physics::algorithms::AlgRemez::approx(const bigfloat x)
{
    bigfloat yn, yd;
    int i;

    // Work backwards toward the constant term.
    yn = param[n];  // Highest order numerator coefficient
    for (i = n - 1; i >= 0; i--)
        yn = x * yn + param[i];
    yd = x + param[n + d];  // Highest degree coefficient = 1.0
    for (i = n + d - 1; i > n; i--)
        yd = x * yd + param[i];

    return (yn / yd);
}

// Compute size and sign of the approximation error at x
bigfloat physics::algorithms::AlgRemez::getErr(bigfloat x, int* sign)
{
    bigfloat e, f;

    f = func(x);
    e = approx(x) - f;
    if (f != 0)
        e /= f;
    if (e < (bigfloat)0.0) {
        *sign = -1;
        e     = -e;
    } else
        *sign = 1;

    return (e);
}

// Calculate function required for the approximation.
bigfloat physics::algorithms::AlgRemez::func(const bigfloat x)
{
    bigfloat z = (bigfloat)power_num / (bigfloat)power_den;
    bigfloat y;

    if (x == (bigfloat)1.0)
        y = (bigfloat)1.0;
    else
        y = pow_bf(x, z);

    if (a_length > 0) {
        bigfloat sum = 0l;
        for (int j = 0; j < a_length; j++)
            sum += a[j] * pow_bf(x, a_power[j]);
        return y * exp_bf(sum);
    } else {
        return y;
    }
}

// Solve the system AX=B
int physics::algorithms::AlgRemez::simq(bigfloat A[], bigfloat B[], bigfloat X[], int n)
{
    int i, j, ij, ip, ipj, ipk, ipn;
    int idxpiv, iback;
    int k, kp, kp1, kpk, kpn;
    int nip, nkp, nm1;
    bigfloat em, q, rownrm, big, size, pivot, sum;
    bigfloat* aa;

    // simq() work vector
    int* IPS = new int[(neq) * sizeof(int)];

    nm1 = n - 1;
    // Initialize IPS and X

    ij = 0;
    for (i = 0; i < n; i++) {
        IPS[i] = i;
        rownrm = 0.0;
        for (j = 0; j < n; j++) {
            q = abs_bf(A[ij]);
            if (rownrm < q)
                rownrm = q;
            ++ij;
        }
        if (rownrm == (bigfloat)0l) {
            logger.warn() << "In AlgRemez::simq. simq rownrm=0";
            delete[] IPS;
            return (1);
        }
        X[i] = (bigfloat)1.0 / rownrm;
    }

    for (k = 0; k < nm1; k++) {
        big    = 0.0;
        idxpiv = 0;

        for (i = k; i < n; i++) {
            ip   = IPS[i];
            ipk  = n * ip + k;
            size = abs_bf(A[ipk]) * X[ip];
            if (size > big) {
                big    = size;
                idxpiv = i;
            }
        }

        if (big == (bigfloat)0l) {
            logger.warn() << "In AlgRemez::simq. simq big=0";
            delete[] IPS;
            return (2);
        }
        if (idxpiv != k) {
            j           = IPS[k];
            IPS[k]      = IPS[idxpiv];
            IPS[idxpiv] = j;
        }
        kp    = IPS[k];
        kpk   = n * kp + k;
        pivot = A[kpk];
        kp1   = k + 1;
        for (i = kp1; i < n; i++) {
            ip     = IPS[i];
            ipk    = n * ip + k;
            em     = -A[ipk] / pivot;
            A[ipk] = -em;
            nip    = n * ip;
            nkp    = n * kp;
            aa     = A + nkp + kp1;
            for (j = kp1; j < n; j++) {
                ipj    = nip + j;
                A[ipj] = A[ipj] + em * *aa++;
            }
        }
    }
    kpn = n * IPS[n - 1] + n - 1;  // last element of IPS[n] th row
    if (A[kpn] == (bigfloat)0l) {
        logger.warn() << "In AlgRemez::simq. simq A[kpn]=0";
        delete[] IPS;
        return (3);
    }

    ip   = IPS[0];
    X[0] = B[ip];
    for (i = 1; i < n; i++) {
        ip  = IPS[i];
        ipj = n * ip;
        sum = 0.0;
        for (j = 0; j < i; j++) {
            sum += A[ipj] * X[j];
            ++ipj;
        }
        X[i] = B[ip] - sum;
    }

    ipn      = n * IPS[n - 1] + n - 1;
    X[n - 1] = X[n - 1] / A[ipn];

    for (iback = 1; iback < n; iback++) {
        // i goes (n-1),...,1
        i   = nm1 - iback;
        ip  = IPS[i];
        nip = n * ip;
        sum = 0.0;
        aa  = A + nip + i + 1;
        for (j = i + 1; j < n; j++)
            sum += *aa++ * X[j];
        X[i] = (X[i] - sum) / A[nip + i];
    }

    delete[] IPS;
    return (0);
}

// Calculate the roots of the approximation
int physics::algorithms::AlgRemez::root()
{
    long i, j;
    bigfloat x, dx = 0.05;
    bigfloat upper = 1, lower = -100000;
    bigfloat tol = 1e-20;

    bigfloat* poly = new bigfloat[neq + 1];

    // First find the numerator roots
    for (i = 0; i <= n; i++)
        poly[i] = param[i];
    // for (i=0; i<=n; i++) printf("%d %e\n", i, (double)poly[i]);

    for (i = n - 1; i >= 0; i--) {
        roots[i] = rtnewt(poly, i + 1, lower, upper, tol);
        // printf("root[%d] = %e\n", i, (double)roots[i]);
        if (roots[i] == 0.0) {
            logger.fatal() << "In AlgRemez::root. Failure to converge on root " << i + 1 << "/" << n;
            return 0;
        }
        poly[0] = -poly[0] / roots[i];
        for (j = 1; j <= i; j++)
            poly[j] = (poly[j - 1] - poly[j]) / roots[i];
    }

    // Now find the denominator roots
    poly[d] = 1l;
    for (i = 0; i < d; i++)
        poly[i] = param[n + 1 + i];
    // for (i=0; i<=d; i++) printf("%d %e\n", i, (double)poly[i]);

    for (i = d - 1; i >= 0; i--) {
        poles[i] = rtnewt(poly, i + 1, lower, upper, tol);
        // printf("pole[%d] = %e\n", i, (double)poles[i]);
        if (poles[i] == 0.0) {
            logger.fatal() << "In AlgRemez::root. Failure to converge on pole " << i + 1 << "/" << d;
            return 0;
        }
        poly[0] = -poly[0] / poles[i];
        for (j = 1; j <= i; j++)
            poly[j] = (poly[j - 1] - poly[j]) / poles[i];
    }

    norm = param[n];
    // printf("Normalisation constant is %e\n",(double)norm);
    // for (i=0; i<n; i++) printf("%ld root = %e\n",i,(double)roots[i]);
    // for (i=0; i<d; i++) printf("%ld pole = %e\n",i,(double)poles[i]);

    delete[] poly;

    return 1;
}

// Evaluate the polynomial
bigfloat physics::algorithms::AlgRemez::polyEval(bigfloat x, bigfloat* poly, long size)
{
    bigfloat f = poly[size];
    for (int i = size - 1; i >= 0; i--)
        f = f * x + poly[i];
    return f;
}

/*
// Evaluate the polynomial using complex numbers
complex_bf AlgRemez::polyEval(complex_bf x,complex_bf *poly, long size) {
  complex_bf f = poly[size];
  for (long i=size-1; i>=0; i--) f = f*x + poly[i];
  return f;
}
*/
// Evaluate the differential of the polynomial
bigfloat physics::algorithms::AlgRemez::polyDiff(bigfloat x, bigfloat* poly, long size)
{
    bigfloat df = (bigfloat)size * poly[size];
    for (int i = size - 1; i > 0; i--)
        df = df * x + (bigfloat)i * poly[i];
    return df;
}

// Newton's method to calculate roots
bigfloat physics::algorithms::AlgRemez::rtnewt(bigfloat* poly, long i, bigfloat x1, bigfloat x2, bigfloat xacc)
{
    int j;
    bigfloat df, dx, f, rtn;

    rtn = (bigfloat)0.5 * (x1 + x2);
    for (j = 1; j <= JMAX; j++) {
        f  = polyEval(rtn, poly, i);
        df = polyDiff(rtn, poly, i);
        dx = f / df;
        rtn -= dx;
        // if ((x1-rtn)*(rtn-x2) < (bigfloat)0.0)
        // printf("Jumped out of brackets in rtnewt\n");
        if (abs_bf(dx) < xacc)
            return rtn;
    }
    logger.fatal() << "In AlgRemez::rtnewt. Maximum number of iterations exceeded in rtnewt.";
    return 0.0;
}

// Evaluate the partial fraction expansion of the rational function
// with res roots and poles poles.  Result is overwritten on input
// arrays.
void physics::algorithms::AlgRemez::pfe(bigfloat* res, bigfloat* poles, bigfloat norm)
{
    int i, j, small;
    bigfloat temp;
    bigfloat* numerator   = new bigfloat[n];
    bigfloat* denominator = new bigfloat[d];

    // Construct the polynomials explicitly
    for (i = 1; i < n; i++) {
        numerator[i]   = 0l;
        denominator[i] = 0l;
    }
    numerator[0]   = 1l;
    denominator[0] = 1l;

    for (j = 0; j < n; j++) {
        for (i = n - 1; i >= 0; i--) {
            numerator[i] *= -res[j];
            denominator[i] *= -poles[j];
            if (i > 0) {
                numerator[i] += numerator[i - 1];
                denominator[i] += denominator[i - 1];
            }
        }
    }

    // Convert to proper fraction form.
    // Fraction is now in the form 1 + n/d, where O(n)+1=O(d)
    for (i = 0; i < n; i++)
        numerator[i] -= denominator[i];

    // Find the residues of the partial fraction expansion and absorb the
    // coefficients.
    for (i = 0; i < n; i++) {
        res[i] = 0l;
        for (j = n - 1; j >= 0; j--) {
            res[i] = poles[i] * res[i] + numerator[j];
        }
        for (j = n - 1; j >= 0; j--) {
            if (i != j)
                res[i] /= poles[i] - poles[j];
        }
        res[i] *= norm;
    }

    // res now holds the residues
    j = 0;
    for (i = 0; i < n; i++)
        poles[i] = -poles[i];

    // Move the ordering of the poles from smallest to largest
    for (j = 0; j < n; j++) {
        small = j;
        for (i = j + 1; i < n; i++) {
            if (poles[i] < poles[small])
                small = i;
        }
        if (small != j) {
            temp         = poles[small];
            poles[small] = poles[j];
            poles[j]     = temp;
            temp         = res[small];
            res[small]   = res[j];
            res[j]       = temp;
        }
        // printf("Residue = %e, Pole = %e\n", (double)res[j], (double)poles[j]);
    }

    delete[] numerator;
    delete[] denominator;
}

double physics::algorithms::AlgRemez::evaluateApprox(double x)
{
    return (double)approx((bigfloat)x);
}

double physics::algorithms::AlgRemez::evaluateInverseApprox(double x)
{
    return 1.0 / (double)approx((bigfloat)x);
}

double physics::algorithms::AlgRemez::evaluateFunc(double x)
{
    return (double)func((bigfloat)x);
}

double physics::algorithms::AlgRemez::evaluateInverseFunc(double x)
{
    return 1.0 / (double)func((bigfloat)x);
}
