#ifndef CompositeResiduosity_HPP
#define CompositeResiduosity_HPP

#include "Paillier.hpp"

class CompositeResiduosity : public Paillier {
    public:
    CompositeResiduosity() : Paillier() {};
    ~CompositeResiduosity() {};

    void keygen (
        Publickey& pk,
        Secretkey& sk
    );
    
    Ciphertext& encrypt (
        Plaintext& pt,
        Publickey& pk
    );

    Plaintext& decrypt(
        Ciphertext& ct,
        Publickey& pk,
        Secretkey& sk
    );

    Plaintext& decryptCRT(
        Ciphertext& ct,
        Publickey& pk,
        Secretkey& sk
    );

}; // end of class

// out of line class member definitions
inline void CompositeResiduosity::keygen(Publickey& pk, Secretkey& sk) {
    Paillier::keygen(pk, sk);
    
    // precompute hp
    mpz_sub_ui(temp1, p, 1);
    mpz_mul(temp2, p, p);
    mpz_powm(hp, g, temp1, temp2); // g^(p-1) % p^2
    mpz_sub_ui(hp, hp, 1);
    mpz_divexact(hp, hp, p);
    buffer = mpz_invert(hp, hp, p);
    
    // precompute hq
    mpz_sub_ui(temp1, q, 1);
    mpz_mul(temp3, q, q);
    mpz_powm(hq, g, temp1, temp3); // g^(q-1) % q^2
    mpz_sub_ui(hq, hq, 1);
    mpz_divexact(hq, hq, q);
    buffer = mpz_invert(hq, hq, q);
    
    // precompute L(g^lam % n^2)^-1
    mpz_powm(temp1, g, lam, nn);
    mpz_sub_ui(temp1, temp1, 1);
    mpz_divexact(temp1, temp1, n);
    buffer = mpz_invert(precomp, temp1, n);

    // set keys
    pk.set(n, nn, g);
    sk.set(p, q, temp2, temp3);
    sk.set(lam);
}

inline Ciphertext& CompositeResiduosity::encrypt(Plaintext& pt, Publickey& pk) {
    // get pt and pk
    pt.get(m);
    pk.get(n, nn, g);
    
    // choose r from [0, n-1]
    mpz_urandomm(r, rands, n);

    // compute ciphertext c = (g^m * r^n) % n^2
    mpz_powm(c, g, m, nn);
    mpz_powm(temp1, r, n, nn);
    mpz_mul(c, c, temp1);
    mpz_mod(c, c, nn);
        
    // set ciphertext
    ct.set(c);
    return ct;
}

inline Plaintext& CompositeResiduosity::decrypt(Ciphertext& ct, Publickey& pk, Secretkey& sk) {
    // get c, pk and sk
    ct.get(c);
    pk.get(n, nn);
    sk.get(lam);
    
    // compute L(c^lam % n^2)
    mpz_powm(temp1, c, lam, nn);
    mpz_sub_ui(temp1, temp1, 1);
    mpz_divexact(temp1, temp1, n);
    
    // compute d = (L(.) * precomp) % n
    mpz_mul(temp1, temp1, precomp);
    mpz_mod(d, temp1, n);
    
    // set plaintext
    pt.set(d);
    return pt;
}

inline Plaintext& CompositeResiduosity::decryptCRT(Ciphertext& ct, Publickey& pk, Secretkey& sk) {
    // get c, pk and sk
    ct.get(c);
    sk.get(p, q, temp2, temp3);
    
    // compute mp
    mpz_sub_ui(temp1, p, 1);
    mpz_powm(mp, c, temp1, temp2);
    mpz_sub_ui(mp, mp, 1);
    mpz_divexact(mp, mp, p);
    mpz_mul(mp, mp, hp);
    mpz_mod(mp, mp, p);
    
    // compute mq
    mpz_sub_ui(temp1, q, 1);
    mpz_powm(mq, c, temp1, temp3);
    mpz_sub_ui(mq, mq, 1);
    mpz_divexact(mq, mq, q);
    mpz_mul(mq, mq, hq);
    mpz_mod(mq, mq, q);
    
    // compute m from mp and mq
    Paillier::CRT(d, mp, mq, p, q);
    
    // set plaintext
    pt.set(d);
    return pt;
}

#endif // CompositeResiduosity_HPP