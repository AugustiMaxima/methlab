
class polynomial{
    int deg;//# of terms
    int coef*;
    public:
    polynomial(int,int*);//an unorthodox choice perhaps, but efficient and lightweight
			//takes a int pointer and borrow the memory. Do not free it
    ~polynomial();
    int& operator[](const int);
    polynomial operator+(const polynomial&) const;//consumes and produce a new value. Functional style
    polynomial operator*(const polynomial&) const;//ditto
    polynomial operator-() const;
    polynomial operator-(const polynomial&) const;
};

