#ifndef COMPLEX_H
#define COMPLEX_H

class Complex
{
    public:
        Complex(){}
        Complex(double real,double imag) : r(real), i(imag) {}
        Complex operator+(Complex add)
        {
            return Complex(add.r+r,add.i+i);
        }
        Complex operator-(Complex sub)
        {
            return Complex(r-sub.r,i-sub.i);
        }
        Complex operator*(Complex mul)
        {
            return Complex(r*mul.r-i*mul.i,i*mul.r+r*mul.i);
        }
        double abs()
        {
            return r*r+i*i;
        }

        double r=0,i=0;
};

#endif // COMPLEX_H
