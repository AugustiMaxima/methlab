#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H


class polynomial{
    int deg;//# of terms
    int *coef;
    public:
    polynomial(int d,int* c= new int[d+1]);//an unorthodox choice perhaps, but efficient and lightweight
			//takes a int pointer and borrow the memory. Do not free it
			//without a pointer; it will simply allocate the amount required
    ~polynomial();
    polynomial(const polynomial&);
    polynomial(polynomial&&);
    polynomial& operator=(const polynomial&);
    polynomial& operator=(polynomial&&);
    int& operator[](const int);
    polynomial operator+(const polynomial&) const;//consumes and produce a new value. Functional style
    polynomial operator*(const polynomial&) const;//ditto
    //polynomial operator-() const;		    //I'm lazy, this is EZ. TODO
    //polynomial operator-(const polynomial&) const;
};


#endif
