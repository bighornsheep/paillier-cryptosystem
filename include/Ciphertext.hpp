#ifndef Ciphertext_HPP
#define Ciphertext_HPP

class Ciphertext {
    protected:
        mpz_t lc;
    public:
        Ciphertext()  {
            mpz_init(lc);
        };

        ~Ciphertext() {
            mpz_clear(lc);
        };
        
        void get(mpz_t c);
        void set(const mpz_t c);
}; // end of class

inline void Ciphertext::get(mpz_t c) {
    mpz_set(c, lc);
}

inline void Ciphertext::set(const mpz_t c) {
    mpz_set(lc, c);
}

#endif // Ciphertext_HPP