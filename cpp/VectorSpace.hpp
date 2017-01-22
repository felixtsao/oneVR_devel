////////////////////////////////////////////////////////////////////////////////
//
// Author: Felix Tsao, hello@felixtsao.com
// Summary: Basic linear algebra Vector classes and operations
//
////////////////////////////////////////////////////////////////////////////////

#ifndef VECTORSPACE
#define VECTORSPACE

#include <iostream>
#include <cmath>

#define D_ERROR() std::cout << "Non-conformal dimensions detected" << std::endl;

///////////////////////////////// Classes //////////////////////////////////////


// N-dimensional Vector
// -----------------------------------------------------------------------------
class Vector {

    public:

        // Constructor and destructor
        inline Vector(int n);
        inline ~Vector();

        // Indexing and access
        inline double operator[](int i) const; // Read
        inline double& operator[](int i); // Write

        // Assignment
        inline Vector& operator=(const Vector &v);

        // Debugging
        inline int size() const;
        inline void print() const;


    private:

        // Stored values
        double * element;
        int dim = 0;

};


// MxN Matrix (rows, columns)
// -----------------------------------------------------------------------------
class Matrix {

    public:

        // Constructors and destructor
        inline Matrix(int m, int n);
        inline Matrix(Vector v); // Column Vector
        inline ~Matrix();

        // Indexing and Access
        inline double operator()(int i, int j) const;
        inline double &operator()(int i, int j);
        inline Vector row(int i) const;
        inline Vector col(int j) const;

        // Assignment
        inline Matrix& operator=(const Matrix &A);


        // Debugging
        inline int nRows() const;
        inline int nCols() const;
        inline void print() const;

    private:

        // Matrix enrtries
        double * element;
        int rows = 0, cols = 0;

        // Linear access
        inline double operator[](int i) const; // Read
        inline double& operator[](int i); // Write

};


///////////////////////////// Vector Space /////////////////////////////////////

// Addition
inline Vector operator+(const Vector &a, const Vector &b);
inline Vector operator-(const Vector &a, const Vector &b);

// Scalar Multiplication
inline Vector operator*(double c, const Vector &a);
inline Vector operator/(const Vector &a, double c);

// Dot Product
inline double dot(const Vector &a, const Vector &b);

// Euclidean Distance
inline double L2Norm(const Vector &a);

// Matrix Multiplication
inline Matrix operator*(const Matrix &A, const Matrix &B);

// Matrix-Vector Multiplication
inline Vector operator*(const Matrix &A, const Vector &x);

// Linear solver Ax = b
inline Vector operator/(const Vector &b, const Matrix &A);


/////////////////////////// Vector Implementation //////////////////////////////


// Constructor and Destructor
// -----------------------------------------------------------------------------

inline Vector::Vector(int n){
    dim = n;
    element = (double *) malloc(dim * sizeof(double));
    //element = new double[dim];
    for (int i = 0; i < dim; i++) element[i] = 0.0;
}

inline Vector::~Vector(){ free(element); }


// Indexing and access
// -----------------------------------------------------------------------------

inline double Vector::operator[](int i) const{ // Read
    if(i >= dim) D_ERROR();
    return element[i];
}

inline double& Vector::operator[](int i){ // Write
    if(i >= dim) D_ERROR();
    return element[i];
}


// Assignment
// -----------------------------------------------------------------------------

inline Vector& Vector::operator=(const Vector &v){
    if(this->size() != v.size()) D_ERROR();
    for (int i = 0; i < this->size(); i++) element[i] = v[i];
    return *this;
}


// Dimension
// -----------------------------------------------------------------------------
inline int Vector::size() const { return dim; }


// -----------------------------------------------------------------------------
// Vector Space
// -----------------------------------------------------------------------------


// Addition (a + b)
// -----------------------------------------------------------------------------
inline Vector operator+(const Vector &a, const Vector &b){ // Addition (a + b)

    if(a.size() != b.size()) D_ERROR();

    int n = a.size();

    Vector sum(n);

    for (int i = 0; i < n; i++) sum[i] = a[i] + b[i];

    return sum;

}

// Subtraction (a - b)
// -----------------------------------------------------------------------------
inline Vector operator-(const Vector &a, const Vector &b){

    if(a.size() != b.size()) D_ERROR();

    int n = a.size();

    Vector diff(n);

    for (int i = 0; i < n; i++) diff[i] = a[i] - b[i];

    return diff;

}

// Scalar Multiplication (c * a)
// -----------------------------------------------------------------------------
inline Vector operator*(double c, const Vector &a){

    Vector v(a.size());

    for (int i = 0; i < a.size(); i++) v[i] = c * a[i];

    return v;

}

// Element-wise Division (a / c)
// -----------------------------------------------------------------------------
inline Vector operator/(const Vector &a, double c){

    Vector v(a.size());

    for (int i = 0; i < a.size(); i++) v[i] = a[i] / c;

    return v;
}

// Dot product (a dot b)
// -----------------------------------------------------------------------------
inline double dot(const Vector &a, const Vector &b){

    if(a.size() != b.size()) D_ERROR();

    double product = 0;

    for (int i = 0; i < a.size(); i++) product += a[i] * b[i];

    return product;

}

// Euclidean Distance, Magnitude |a|
// -----------------------------------------------------------------------------
inline double L2Norm(const Vector &a){

    double squaredSum = 0;

    for (int i = 0; i < a.size(); i++) squaredSum += a[i] * a[i];

    return sqrt(squaredSum);

}


// Debugging
// -----------------------------------------------------------------------------
inline void Vector::print() const {
    std::cout << "(" << element[0];
    for(int i = 1; i < dim; i++) std::cout << ", " << element[i];
    std::cout << ")" << std::endl;
}


/////////////////////////// Matrix Implementation //////////////////////////////

// Constructors and Destructor
// -----------------------------------------------------------------------------

inline Matrix::Matrix(int m, int n){
    rows = m;
    cols = n;
    element = (double *) malloc(m * n * sizeof(double));
    for (int i = 0; i < m * n; i++) element[i] = 0.0;
}

inline Matrix::Matrix(Vector v){ // Column vector
    rows = v.size();
    cols = 1;
    element = (double *) malloc(rows * sizeof(double));
    for (int i = 0; i < rows; i++) element[i] = v[i];
}

inline Matrix::~Matrix(){ free(element); }

// Indexing and access (Column major)
// -----------------------------------------------------------------------------

// Private read and write
inline double Matrix::operator[](int i) const { return element[i]; }
inline double &Matrix::operator[](int i) { return element[i]; }

// Public read
inline double Matrix::operator()(int i, int j) const {
    return element[j * rows + i];
}

// Public write
inline double &Matrix::operator()(int i, int j){
    return element[j * rows + i];
}

// Get number of rows and columns of matrix
inline int Matrix::nRows() const { return rows; }
inline int Matrix::nCols() const { return cols; }

// Get row from matrix
inline Vector Matrix::row(int i) const {
    Vector row(cols);
    for (int j = 0; j < cols; j++) {
        row[j] = element[j * rows + i];
    }
    return row;
}

// Get column from matrix
inline Vector Matrix::col(int j) const {
    Vector col(rows);
    for (int i = 0; i < rows; i++) {
        col[i] = element[j * rows + i];
    }
    return col;
}

// Assignment
inline Matrix& Matrix::operator=(const Matrix &A){

    if(rows != A.nRows() || cols != A.nCols()) D_ERROR();

    if(rows == 0 || cols == 0)
        element = (double *) malloc(A.nRows() * A.nCols() * sizeof(double));

    rows = A.nRows();
    cols = A.nCols();

    for (int i = 0; i < rows * cols; i++) {
        element[i] = A[i];
    }

    return *this;

}


// -----------------------------------------------------------------------------
// Vector Space
// -----------------------------------------------------------------------------

// Matrix Multiplication (A * B)
// -----------------------------------------------------------------------------
inline Matrix operator*(const Matrix &A, const Matrix &B){

    if(A.nCols() != B.nRows()) D_ERROR();

    Matrix M(A.nRows(), B.nCols());

    for (int i = 0; i < A.nRows(); i++){
        for (int j = 0; j < B.nCols(); j++){
            M(i, j) = dot(A.row(i), B.col(j));
        }
    }

    return M;

}


// Matrix-Vector Multiplication (A * x)
// -----------------------------------------------------------------------------
inline Vector operator*(const Matrix &A, const Vector &x){

    if(A.nCols() != x.size()) D_ERROR();

    Vector b(x.size());
    for (int i = 0; i < x.size(); i++) b[i] = dot(A.row(i), x);
    return b;

}


// Linear solver Ax = b, solves for x
// -----------------------------------------------------------------------------
inline Vector operator/(const Vector &b, const Matrix &A){

    if(A.nCols() != b.size()) D_ERROR();

    int rows = A.nRows();
    int cols = A.nCols();
    int dim = fmin(rows, cols);

    // Solve via LU decomposition i.e.
    // Ax = b => LUx = b

    Matrix U(rows, cols); // Upper triangular matrix
    U = A;
    //std::cout << "Matrix U:" << std::endl; U.print();

    Matrix L(rows, cols); // Lower triangular matrix
    for (int k = 0; k < rows; k++) L(k,k) = 1; // Unit-lower triangular

    // Eliminate lower diagonal of "Upper" U matrix
    for (int j = 0; j < cols - 1; j++) {
        Matrix G(rows, cols); // Intermediate Gauss Transform, temp
        for (int k = 0; k < rows; k++) G(k,k) = 1;
        for (int i = j + 1; i < rows; i++) {
            L(i,j) = U(i,j) / U(j,j);
            G(i,j) = -L(i,j);
        }
        //std::cout << "Matrix G:" << std::endl; G.print();
        U = G * U;
        //std::cout << "Matrix U:" << std::endl; U.print();
        //std::cout << "" << std::endl;
    }

    //std::cout << "Matrix L:" << std::endl; L.print();

    // Solve Ly = b for y...
    Vector y(dim);
    y[0] = b[0] / L(0,0);

    // ...by foward-solve
    for (int i = 0; i < rows; i++) {
        double subs = 0;
        for (int j = 0; j < i; j++) subs += L(i,j) * y[j];
        y[i] = (b[i] - subs) / L(i,i);
    }

    //std::cout << "y  = "; y.print();

    // Solve Ux = y...
    Vector x(rows);
    x[rows - 1] = y[rows - 1] / U(rows - 1, rows - 1);

    // ...by backwards-solve
    for (int i = rows - 2; i > -1; i--) {
        double subs = 0;
        for (int j = (rows - 1); j > i; j--) subs += U(i,j) * x[j];
        x[i] = (y[i] - subs) / U(i,i);
    }

    //std::cout << "x  = "; x.print();

    return x;
} // Ax = b


// -----------------------------------------------------------------------------
// Debugging
inline void Matrix::print() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << element[j * rows + i] << " ";
        }
        std::cout << std::endl;
    }
}





#endif
