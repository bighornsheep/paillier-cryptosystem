#ifndef Secretkey_HPP
#define Secretkey_HPP

class Secretkey {
    protected:
    mpz_t llam;
    mpz_t lalpha;    
    mpz_t lp;
    mpz_t lq;
    mpz_t lpp;
    mpz_t lqq;
    
    public:
    Secretkey()  { mpz_inits(llam, lalpha, lp, lq, lpp, lqq, NULL);  };
    ~Secretkey() { mpz_clears(llam, lalpha, lp, lq, lpp, lqq, NULL); };
    
    void set (const mpz_t lam);
    void set1(const mpz_t alpha);
    void set (const mpz_t p, const mpz_t q, const mpz_t pp, const mpz_t qq);

    void get (mpz_t lam);
    void get1(mpz_t alpha);
    void get (mpz_t p, mpz_t q, mpz_t pp, mpz_t qq);    

}; // end of class


inline void Secretkey::set(const mpz_t lam) {
    mpz_set(llam, lam);
}

inline void Secretkey::set1(const mpz_t alpha) {
    mpz_set(lalpha, alpha);
}

inline void Secretkey::set(const mpz_t p, const mpz_t q, const mpz_t pp, const mpz_t qq) {
    mpz_set(lp,  p);
    mpz_set(lq,  q);
    mpz_set(lpp, pp);
    mpz_set(lqq, qq);
}

inline void Secretkey::get(mpz_t lam) {
    mpz_set(lam, llam);
}

inline void Secretkey::get1(mpz_t alpha) {
    mpz_set(alpha, lalpha);
}

inline void Secretkey::get(mpz_t p, mpz_t q, mpz_t pp, mpz_t qq) {
    mpz_set(p,  lp);
    mpz_set(q,  lq);
    mpz_set(pp, lpp);
    mpz_set(qq, lqq);
}

#endif // Secretkey_HPP