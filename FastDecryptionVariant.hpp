/**
 *
 * @author - Parthasarathi Das
 * @brief  - A class for the Paillier cryptosystem variant with fast decryption
 */

#ifndef FastDecryptionVariant_HPP
#define FastDecryptionVariant_HPP

#include "Paillier.hpp"

class FastDecryptionVariant : public Paillier {
    public:
    FastDecryptionVariant(): Paillier() {};
    ~FastDecryptionVariant() {};
    
    void keygen(
        Publickey& pk,
        Secretkey& sk
    );

    Ciphertext& encrypt(
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
inline void FastDecryptionVariant::keygen(Publickey& pk, Secretkey& sk) {
    Paillier::keygen(pk, sk);

	// generate sk alpha from [1, lam]
    /*
    mpz_urandomm(alpha, rands, lam);
	mpz_mod(temp1, lam, alpha);
	while (mpz_cmp_ui(temp1, 0) != 0) {
		mpz_urandomm(alpha, rands, lam);
		mpz_mod(temp1, lam, alpha);		
	}
    */
    
	mpz_divexact_ui (alpha, lam, 8);
	mpz_divexact(temp1, lam, alpha);
	mpz_powm(g, g, temp1, nn);
	
    // pre-compute hp
    mpz_mul(temp2, p, p);
    mpz_powm(hp, g, alpha, temp2); // g^alpha % p^2
    mpz_sub_ui(hp, hp, 1);
    mpz_divexact(hp, hp, p);
    buffer = mpz_invert(hp, hp, p);
    
    // pre-compute hq
    mpz_mul(temp3, q, q);
    mpz_powm(hq, g, alpha, temp3); // g^alpha % q^2
    mpz_sub_ui(hq, hq, 1);
    mpz_divexact(hq, hq, q);
    buffer = mpz_invert(hq, hq, q);
    
    // precompute L(g^alpha % n^2)^-1
    mpz_powm(temp1, g, alpha, nn);
    mpz_sub_ui(temp1, temp1, 1);
    mpz_divexact(temp1, temp1, n);
    buffer = mpz_invert(precomp, temp1, n);
    
    // set keys
    pk.set (n, nn, g);
    sk.set (p, q, temp2, temp3);
    sk.set1(alpha);
}

inline Ciphertext& FastDecryptionVariant::encrypt(Plaintext& pt, Publickey& pk) {
    // get pt and pk
    pt.get(m);
    pk.get(n, nn, g);
    
    // choose r from [0, n-1]
    mpz_urandomm(r, rands, n);
    
    // compute ciphertext c = (g^(m+nr)) % n^2
    mpz_mul(temp1, r, n);
    mpz_add(temp1, temp1, m);
    mpz_powm(c, g, temp1, nn);
        
    // set ciphertext
    ct.set(c);
    return ct;
}

inline Plaintext& FastDecryptionVariant::decrypt(Ciphertext& ct, Publickey& pk, Secretkey& sk) {
    // get c, pk and sk
    ct.get(c);
    sk.get1(alpha);
    pk.get(n, nn);

    // compute L(c^alpha % n^2)
    mpz_powm(temp1, c, alpha, nn);        
    mpz_sub_ui(temp1, temp1, 1);
    mpz_divexact(temp1, temp1, n);
    
    // compute d = (L(.) * precomp) % n
    mpz_mul(temp1, temp1, precomp);
    mpz_mod(d, temp1, n);
    
    // set plaintext
    pt.set(d);
    return pt;
}


inline Plaintext& FastDecryptionVariant::decryptCRT(Ciphertext& ct, Publickey& pk, Secretkey& sk) {
    // get c, pk and sk
    ct.get(c);
    sk.get1(alpha);
    sk.get(p, q, temp2, temp3);
    
    // compute mp
    mpz_powm(mp, c, alpha, temp2);
    mpz_sub_ui(mp, mp, 1);
    mpz_divexact(mp, mp, p);
    mpz_mul(mp, mp, hp);
    mpz_mod(mp, mp, p);
    
    // compute mq
    mpz_powm(mq, c, alpha, temp3);
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

#endif // FastDecryptionVariant_HPP