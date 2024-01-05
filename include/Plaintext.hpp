#ifndef Plaintext_HPP
#define Plaintext_HPP

class Plaintext {
    protected:
        mpz_t lm;
    public:
        Plaintext() {
            mpz_init(lm);
        };

        ~Plaintext() {
            mpz_clear(lm);
        };
        
        void get(mpz_t m) {
            mpz_set(m, lm);
        }
        
        void set(const mpz_t m) {
            mpz_set(lm, m);
        }
}; // end of class


#endif // Plaintext_HPP