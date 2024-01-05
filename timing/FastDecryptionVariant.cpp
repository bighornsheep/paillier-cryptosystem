/**
 * @author - Parthasarathi Das
 * @brief  - Measures the runtimes the Paillier cryptosystem (variant) and writes timing information to files
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <gmp.h>
#include <string.h>
#include <unistd.h>
#include <NTL/ZZ.h>
#include <NTL/RR.h>

#include "../Paillier.hpp"
#include "../FastDecryptionVariant.hpp"

#include "../include/measurement.h"

NTL_CLIENT

int main(int argc, char** argv) {
    int message_bits = strtol(argv[1], NULL, 10);
    int modulus_bits = strtol(argv[2], NULL, 10);
    
    int total_parameters = 1;
    int total_messages = 1000;

    char m_file[PATH_MAX]; // message m
    char N_file[PATH_MAX]; // RSA modulus N
    char p_file[PATH_MAX]; // prime p
    char q_file[PATH_MAX]; // prime q
    char t_file[PATH_MAX]; // time t
    
    FastDecryptionVariant system;
    ofstream timeout;
    RR factor;
    mpz_t p, q, message, decrypted_message, mod;
    gmp_randstate_t rands;

    Publickey pk;
    Secretkey sk;
    Plaintext pt;
    Plaintext* dt; // decrypted text
    Ciphertext* ct;
    
    mpz_inits(p, q, message, decrypted_message, mod, NULL);
    gmp_randinit_default (rands);
    
    // set the name and path of .txt files for reading and  writing
    FILE *inm, *inN, *inp, *inq;
    
    snprintf(m_file, PATH_MAX, "../params/message/%s.txt",   argv[1]);
    snprintf(N_file, PATH_MAX, "../params/modulus/%s_N.txt", argv[2]);
    snprintf(p_file, PATH_MAX, "../params/modulus/%s_p.txt", argv[2]);
    snprintf(q_file, PATH_MAX, "../params/modulus/%s_q.txt", argv[2]);
    snprintf(t_file, PATH_MAX, "output/fdv_%s_%s.txt", argv[2], argv[1]);
    
    // time variables
    Vec<ZZ> ticks;
    Vec<RR> runtime;
    ticks.SetLength(3);
    runtime.SetLength(3);

    // open time file for writing
    timeout.open(t_file);

    // open parameter files
    inm = fopen(m_file, "r");
    inN = fopen(N_file, "r");
    inp = fopen(p_file, "r");
    inq = fopen(q_file, "r");
    
    for (int i = 0; i < total_parameters; i++) {
        // read parameter i
        mpz_inp_str(mod, inN, 10);
        mpz_inp_str(p,   inp, 10);
        mpz_inp_str(q,   inq, 10);
        
        system.init(modulus_bits, mod, p, q);
        system.keygen(pk, sk);
        
        // encrypt and decrpyt for different messages
        for (int j = 0; j < total_messages; j++) {
            // read message j
            mpz_inp_str(message, inm, 16);
            pt.set(message);
    
            // measure encryption
            MEASURE ( ct = &system.encrypt(pt, pk); );
            ticks[0] = ticks[0] + (etime);

            // measure decryption
            MEASURE ( dt = &system.decrypt(*ct, pk, sk); );
            ticks[1] = ticks[1] + (etime);
            dt->get(decrypted_message);
            assert (mpz_cmp(message, decrypted_message) == 0);
            mpz_set_ui(decrypted_message, 0);
            
            // measure CRT decryption
            MEASURE ( dt = &system.decryptCRT(*ct, pk, sk); );
            ticks[2] = ticks[2] + (etime);
            dt->get(decrypted_message);
            assert (mpz_cmp(message, decrypted_message) == 0);
        }
    }
    
    // compute factor before writing timing information to files
    factor = 1000 / (frequency * GHz * total_parameters * total_messages);
    
    // write timing information to file
    for (int k = 0; k < 3; k++) {
        runtime[k] = conv<RR>(ticks[k]) * factor;
        timeout << " & " << round(runtime[k]);
    }
    timeout << "\\\\";

    std::cout << message_bits << " " << modulus_bits << std::endl;
    
    timeout.close();
    fclose(inm);
    fclose(inN);
    fclose(inp);
    fclose(inq);
    
    mpz_clears (p, q, message, decrypted_message, mod, NULL);
    gmp_randclear (rands);
    
    return 0;
}