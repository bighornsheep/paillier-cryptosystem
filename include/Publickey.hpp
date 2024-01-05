#ifndef Publickey_HPP
#define Publickey_HPP

class Publickey {
    protected:
    mpz_t ln;
    mpz_t lnn;
    mpz_t lg;
        
    public:
    Publickey()  { mpz_inits(ln, lnn, lg, NULL);  };
    ~Publickey() { mpz_clears(ln, lnn, lg, NULL); };
        
    void set(const mpz_t n, const mpz_t nn, const mpz_t g);
    void get(mpz_t n, mpz_t nn);
    void get(mpz_t n, mpz_t nn, mpz_t g);
    void get(mpz_t n);    
        
}; // end of class


inline void Publickey::set(const mpz_t n, const mpz_t nn, const mpz_t g) {
    mpz_set(ln,  n);
    mpz_set(lnn, nn);
    mpz_set(lg,  g);
}

inline void Publickey::get(mpz_t n, mpz_t nn) {
    mpz_set(n,  ln);
    mpz_set(nn, lnn);
}

inline void Publickey::get(mpz_t n, mpz_t nn, mpz_t g) {
    mpz_set(n,  ln);
    mpz_set(nn, lnn);
    mpz_set(g,  lg);
}

inline void Publickey::get(mpz_t n) {
    mpz_set(n,  ln);
}

#endif // Publickey_HPP