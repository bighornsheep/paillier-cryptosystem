#ifndef Paillier_HPP
#define Paillier_HPP

#include "include/Publickey.hpp"
#include "include/Secretkey.hpp"
#include "include/Plaintext.hpp"
#include "include/Ciphertext.hpp"

class Paillier {
    public:
    Paillier();
    ~Paillier();
    
    void init(
        const int MS,
        const mpz_t modulus,
        const mpz_t prime1,
        const mpz_t prime2
    );

    void CRT(
        mpz_t x,
        const mpz_t a,
        const mpz_t b,
        const mpz_t m,
        const mpz_t n
    );

    void keygen(
        Publickey& pk,
        Secretkey& sk
    );
    
    
    Ciphertext& evalsum(
        Publickey& pk,
        Ciphertext& ct1,
        Ciphertext& ct2
    );

    Ciphertext& evalscal(
        Publickey& pk,
        Ciphertext& ct,
        const mpz_t alpha
    );

    protected:
    int buffer, modbits, bits;
    mpz_t p, q, n, nn, lam, alpha, r, g, c, m, d, seed, precomp;
    mpz_t hp, hq, mp, mq, crts, crtt, crtq, crtr, temp1, temp2, temp3;
    gmp_randstate_t rands;

    Plaintext pt;
    Ciphertext ct;
};



// out of line class member definitions
inline Paillier::Paillier() {
    mpz_inits(p, q, n, nn, lam, alpha, r, g, c, m, d, seed, precomp, hp, hq, mp, mq, temp1, temp2, temp3, NULL);
    mpz_inits(crts, crtt, crtq, crtr, NULL);
    gmp_randinit_default(rands);
}


inline Paillier::~Paillier() {
    mpz_clears(p, q, n, nn, lam, alpha, r, g, c, m, d, seed, precomp, hp, hq, mp, mq, temp1, temp2, temp3, NULL);
    mpz_clears(crts, crtt, crtq, crtr, NULL);
    gmp_randclear(rands);
}


inline void Paillier::init(const int MS, const mpz_t modulus, const mpz_t prime1, const mpz_t prime2) {
    modbits = MS;
    
    mpz_set(n, modulus);
    mpz_set(p, prime1);
    mpz_set(q, prime2);
}


inline void Paillier::keygen(Publickey& pk, Secretkey& sk) {
    // compute n^2
    mpz_mul(nn, n, n);
    
    // compute lambda (Carmichael function of n)
    mpz_sub_ui(lam, p, 1);
    mpz_sub_ui(temp1, q, 1);
    mpz_lcm(lam, lam, temp1);
    
	// set g = 2
    mpz_set_ui(g, 2);
}


inline void Paillier::CRT(mpz_t x, const mpz_t a, const mpz_t b, const mpz_t m, const mpz_t n) {
    mpz_gcdext(x, crts, crtt, m, n);
    mpz_sub(crtt, b, a);
    mpz_fdiv_qr (crtq, crtr, crtt, x);
    mpz_lcm(crtt, m, n);
    mpz_mul(x, crtq, crts);
    mpz_mul(x, x, m);
    mpz_add(x, x, a);
    mpz_mod(x, x, crtt);
}


inline Ciphertext& Paillier::evalsum(Publickey& pk, Ciphertext& ct1, Ciphertext& ct2) {
    pk.get(n, nn);
    ct1.get(temp1);
    ct2.get(temp2);
    
    mpz_mul(temp1, temp1, temp2);
    mpz_mod(temp1, temp1, nn);
    
    ct.set(temp1);
    return ct;
}


inline Ciphertext& Paillier::evalscal(Publickey& pk, Ciphertext& ct, const mpz_t alpha) {
    pk.get(n, nn);
    ct.get(temp1);
    
    mpz_powm(temp1, temp1, alpha, nn);
    
    ct.set(temp1);
    return ct;
}

#endif // Paillier_HPP