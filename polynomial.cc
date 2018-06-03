#include "polynomial.h"

//O(1)
//point about the deg and coef
//degree: highest degree like in polynomial
//coef: list of the coefficients from x^0 to x^degree
//ith element for x^i
//this means you need to allocate deg+1 space for ints
polynomial::polynomial(int deg,int* coef):deg{deg},coef{coef}{}

polynomial::~polynomial(){
    delete[] coef;
}

//O(n)
polynomial::polynomial(const polynomial& p):deg{deg},coef{new int[deg+1]}{
    for(int i=0;i<=deg;i++){
	coef[i] = p[i];
    }
}


//O(1) for move constructor
polynomial::polynomial(polynomial&& p):deg{deg},coef{coef}{}

//O(n)
polynomial& polynomial::operator=(const polynomial& p){
    if(p.deg!=deg){	//now, I could have been more lenient and just set the condition to be p.deg>deg without any repurcussions besides overclaimed memory, but this is cleaner
	delete[] coef;
	coef = new int[p.deg+1];
    }
    deg = p.deg;
    for(int i=0;i<=deg;i++)
	coef[i] = p[i];
    return *this;
}

//O(1)
polynomial& polynomial::operator=(polynomial&& p){
    deg = p.deg; //as destructor calls dont rely on deg, we can safely just lose that value
    int* t = coef; //if I tried the same for coef, I'd have a memory leak
    coef = p.coef;
    p.coef = t;//p will do the honour of ridding us the baggage
}

//O(1)
int& polynomial::operator[](const int i){
    return coef[i];
}

//O(max(n,m)) where n is # of coefs for self, m # of coefs for p
polynomial polynomial::operator+(const polynomial& p){
    polynomial& l = p.deg > deg? p : *this;
    polynomial& s = p.deg < deg > p: *this;
    int *nc = new int[l.deg+1];
    int nd = l.deg;
    for(int i=0;i<l=s.deg;i++){
	nc[i] = l[i] + r[i];
    }
    for(int i=s.deg+1;i<=l.deg;i++){
	nc[i] = l[i];
    }
    return polynomial(nd,nc);
}


//side effects: heavy alloc calls, O(n log n) memory usage
//returns a newly alloced int array that the caller will be responsible for freeing
//per static, not intended to be exposed to client
//skips the overhead of copy, constructor and destructor call
static int* fastmultiply(const int* a, const int* b, const int da, const int db){
    int* ab;
    
    if(da==0){//scalar multiplication
	ab = new int[db+1];
	for(int i=0;i<=db;i++){
	    ab[i] = a[0]*b[i];
	}
    }
    else if(db==0){//scalar but with b
	ab = new int[db+1];
	for(int i=0;i<=da;i++){
	    ab[i] = b[0]*a[i];
	}
    }else{//why is the embedded code unindented? symbolically, I'd like to see this as the actual main body of the routine

    int offset = ((da>db?db:da)+1)/2;//base the offset on the polynomial with lower degrees
				    //it's been suggested that padding 0s is the easier solution, but I'd rather not waste instructions
    int *sa = new int[da-offset+1];//summing the lower partition and higher partition of a -> heavy pointer arithmetic
    int *sb = new int[db-offset+1];

//some note -> lower partition of a: 0 ... offset-1, size offset
	    // upper partition of a: offset ... da, size da - offset + 1
//As the offset is the output of the integer division, it's bounded under (dn+1)/2
//This means the partition of length dn+1 - offset >= offset and is therefore the greater set

    for(int i=0;i<offset;i++){
	sa[i] = a[i] + a[offset+i];
	sb[i] = b[i] + b[offset+i];
    }
    for(int i=offset;i<=da-offset;i++){
	sa[i] = a[offset+i];
    }
    for(int i=offset;i<=db-offset;i++){
	sb[i] = b[offset+i];
    }

    //as long as abu
    int* sab = fastmultiply(sa,sb,da-offset,db-offset);
    
    //The sums have officially outlived their usefulness
    delete[] sa;
    delete[] sb;

    //product of lower partition of a and b -> degrees: 2*offset-2
    int* abl = fastmultiply(a,b,offset-1,offset-1);
    //product of upper partition of a and b -> degrees: da+db - 2*offset 
    int* abu = fastmultiply(a+offset,b+offset,da-offset,db-offset);
   
    //subtract products from sab -> refer to (a1+a2)(b1+b2)-a1b1-a2b2 
    for(int i=0;i<=2*offset-2;i++){
	sab[i] - = abl[i] + abu[i];
    }
    for(int i=2*offset-1;i<=da+db-2*offset;i++){
	sab[i] - = abu[i];
    }

 
    //final result
    ab = new int[da+db+1];
    //degree: da+db

    //Tricks for future reference:
    //abl fills immediately from the start
    //sab starts filling at offset
    //abu fills at 2*offset

    for(int i=0;i<=da+db;i++){//I'd inline these variables, but C++17 probably is too new for compilers
	int l = (i<=2*offset-2)?abl[i]:0;
	int m = (i>=offset && i<=da+db-offset)?sab[i-offset]:0;
	int h = (i>=2*offset)?abu[i-2*offset]:0;
	ab[i] = l+m+h;	
    }
    
    //The great purge begins
    delete[] sab;
    delete[] abl;
    delete[] abu;	
    }

    return ab;//the caller can free it

}

//O(n^1.5)? to be verified later, TODO
//The fast integer multiplication algorithm by that Russian guy,
//now adapted for Polynomials
polynomial polynomial::operator*(const polynomial& p){
    int* product = fastmultiply(coef,p.coef,deg,p.deg);
    return polynomial{deg+p.deg,product};
}
