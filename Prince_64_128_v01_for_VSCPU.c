/* YEDITEPE UNIVERSITY - B. SC. STUDENT - ABDULLAH KUSPINAR - ab.kuspinar16@gmail.com - www.abdullahkuspinar.com - */

// #include "header.inc"

// #include <stdint.h>

// #include "cipher.h"
// #include "common.h"
// #include "constants.h"

unsigned int state[8];

unsigned int key[16];

unsigned int roundKeys[24];

unsigned int S0[16];

unsigned int S1[16];

unsigned int RC[96];

unsigned int i;

void SLayer(unsigned int *block)
{
    unsigned int i;
    

    for(i = 0; i < 8; i++)
    {
        block[i] = (((S0[(block[i] >> 4)])) << 4) ^ ((S0[(block[i] & 0x0F)]));
    }
}

void InverseSLayer(unsigned int *block)
{
    unsigned int i;
    

    for(i = 0; i < 8; i++)
    {
        block[i] = (((S1[(block[i] >> 4)])) << 4) ^ ((S1[(block[i] & 0x0F)]));
    }
}

void M0Multiplication(unsigned int *block)
{
    unsigned int temp[2];

    
    temp[1] = (0x07 & (block[1] >> 4)) ^ (0x0B & block[1]) ^ (0x0D & (block[0] >> 4)) ^ (0x0E & block[0]);
    temp[1] = (temp[1] << 4) ^ (0x0B & (block[1] >> 4)) ^ (0x0D & block[1]) ^ (0x0E & (block[0] >> 4)) ^ (0x07 & block[0]);
        
    temp[0] = (0x0D & (block[1] >> 4)) ^ (0x0E & block[1]) ^ (0x07 & (block[0] >> 4)) ^ (0x0B & block[0]);
    temp[0] = (temp[0] << 4) ^ (0x0E & (block[1] >> 4)) ^ (0x07 & block[1]) ^ (0x0B & (block[0] >> 4)) ^ (0x0D & block[0]);

    
    block[1] = temp[1];
    block[0] = temp[0];
}

void M1Multiplication(unsigned int *block)
{
    unsigned int temp[2];

    
    temp[1] = (0x0B & (block[1] >> 4)) ^ (0x0D & block[1]) ^ (0x0E & (block[0] >> 4)) ^ (0x07 & block[0]);
    temp[1] = (temp[1] << 4) ^ (0x0D & (block[1] >> 4)) ^ (0x0E & block[1]) ^ (0x07 & (block[0] >> 4)) ^ (0x0B & block[0]);
        
    temp[0] = (0x0E & (block[1] >> 4)) ^ (0x07 & block[1]) ^ (0x0B & (block[0] >> 4)) ^ (0x0D & block[0]);
    temp[0] = (temp[0] << 4) ^ (0x07 & (block[1] >> 4)) ^ (0x0B & block[1]) ^ (0x0D & (block[0] >> 4)) ^ (0x0E & block[0]);
    
    
    block[1] = temp[1];
    block[0] = temp[0];
}

void SR(unsigned int *block)
{
    unsigned int temp0, temp1;


    /* Shift left column 1 by 1 */
    temp0 = block[7];
    block[7] = (block[7] & 0xF0) ^ (block[5] & 0x0F);
    block[5] = (block[5] & 0xF0) ^ (block[3] & 0x0F);
    block[3] = (block[3] & 0xF0) ^ (block[1] & 0x0F);
    block[1] = (block[1] & 0xF0) ^ (temp0 & 0x0F);


    /* Shift left column 2 by 2 and column 3 by 3 */
    temp0 = block[0];
    temp1 = block[2];

    block[0] = (block[4] & 0xF0) ^ (block[2] & 0x0F);
    block[2] = (block[6] & 0xF0) ^ (block[4] & 0x0F);
    block[4] = (temp0 & 0xF0) ^ (block[6] & 0x0F);
    block[6] = (temp1 & 0xF0) ^ (temp0 & 0x0F);
}

void InverseSR(unsigned int *block)
{
    unsigned int temp0, temp1;


    /* Shift right column 1 by 1 */
    temp0 = block[1];
    block[1] = (block[1] & 0xF0) ^ (block[3] & 0x0F);
    block[3] = (block[3] & 0xF0) ^ (block[5] & 0x0F);
    block[5] = (block[5] & 0xF0) ^ (block[7] & 0x0F);
    block[7] = (block[7] & 0xF0) ^ (temp0 & 0x0F);

    
    /* Shift right column 2 by 2 and column 3 by 3 */
    temp0 = block[6];
    temp1 = block[4];

    block[6] = (block[2] & 0xF0) ^ (block[4] & 0x0F);
    block[4] = (block[0] & 0xF0) ^ (block[2] & 0x0F);
    block[2] = (temp0 & 0xF0) ^ (block[0] & 0x0F);
    block[0] = (temp1 & 0xF0) ^ (temp0 & 0x0F);
}

void AddRoundRoundKeyAndRoundConstant(unsigned int *block, unsigned int *roundKey,  unsigned int *roundConstant)
{
    unsigned int i;


    for(i = 0; i < 8; i++)
    {
        block[i] = block[i] ^ (roundConstant[i]);
        block[i] = block[i] ^ (roundKey[i]);
    }
}

void Round(unsigned int *block, unsigned int *roundKey,  unsigned int *roundConstant)
{   
    /* S-Layer */
    SLayer(block);

    /* M-Layer */
    M0Multiplication(&block[6]);
    M1Multiplication(&block[4]);
    M1Multiplication(&block[2]);
    M0Multiplication(&block[0]);

    /* SR */
    SR(block);

    /* XOR K1, XOR RCi */
    AddRoundRoundKeyAndRoundConstant(block, roundKey, roundConstant);
}

void InverseRound(unsigned int *block, unsigned int *roundKey,  unsigned int *roundConstant)
{
    /* XOR K1, XOR RCi */
    AddRoundRoundKeyAndRoundConstant(block, roundKey, roundConstant);

    /* Inverse SR */
    InverseSR(block);

    /* M-Layer */
    M0Multiplication(&block[6]);
    M1Multiplication(&block[4]);
    M1Multiplication(&block[2]);
    M0Multiplication(&block[0]);

    /* Inverse S-Layer */
    InverseSLayer(block);
}

void Whitening(unsigned int *block, unsigned int *roundKey)
{
    unsigned int i;


    for(i = 0; i < 8; i++)
    {
        block[i] = block[i] ^ (roundKey[i]);
    }
}

void EncryptionPrinceCore(unsigned int *block, unsigned int *roundKeys)
{
    unsigned int i;


    AddRoundRoundKeyAndRoundConstant(block, roundKeys, &RC[0]);

    for(i = 1; i < 6; i++)
    {
        Round(block, roundKeys, &RC[8 * i]);
    }


    /* Middle layer - begin */

    /* S-Layer */
    SLayer(block);

    /* M-Layer */
    M0Multiplication(&block[6]);
    M1Multiplication(&block[4]);
    M1Multiplication(&block[2]);
    M0Multiplication(&block[0]);

    /* Inverse S-Layer */
    InverseSLayer(block);

    /* Middle layer - end */

    
    for(i = 6; i < 11; i++)
    {
        InverseRound(block, roundKeys, &RC[8 * i]);
    }

    AddRoundRoundKeyAndRoundConstant(block, roundKeys, &RC[88]);
}

void Encrypt(unsigned int *block, unsigned int *roundKeys)
{
    Whitening(block, roundKeys);
    EncryptionPrinceCore(block, &roundKeys[16]);
    Whitening(block, &roundKeys[8]);
}

void RunEncryptionKeySchedule(unsigned int *key, unsigned int *roundKeys)
{
    unsigned int i;

    
    for(i = 0; i < 8; i++)
    {
        /* Copy K0 to round keys */
        roundKeys[i] = key[i];
    
        /* Copy K1 to round keys */
        roundKeys[i + 16] = key[i + 8];
    }


    /* Generate K0' */
    for(i = 0; i < 7; i++)
    {
        roundKeys[i + 8] = ((key[i + 1] << 7) & 0x80) ^ (key[i] >> 1);
    }

    roundKeys[15] = ((key[0] << 7) & 0x80) ^ (key[7] >> 1);
    roundKeys[8] = roundKeys[8] ^ ((key[7] >> 7) & 0x01);
}

int main()
{
    
    unsigned int state_init[8] = {0xef, 0xcd, 0x0ab, 0x89, 0x67, 0x45, 0x23, 0x01};

    unsigned int key_init[16] = {0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};

    unsigned int S0_init[16] = {0xB, 0xF, 0x3, 0x2, 0xA, 0xC, 0x9, 0x1, 0x6, 0x7, 0x8, 0x0, 0xE, 0x5, 0xD, 0x4};

    unsigned int S1_init[16] = {0xB, 0x7, 0x3, 0x2, 0xF, 0xD, 0x8, 0x9, 0xA, 0x6, 0x4, 0x0, 0x5, 0xE, 0xC, 0x1};

    unsigned int RC_init[96] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x44, 0x73, 0x70, 0x03, 0x2e, 0x8a, 0x19, 0x13,
        0xd0, 0x31, 0x9f, 0x29, 0x22, 0x38, 0x09, 0xa4,
        0x89, 0x6c, 0x4e, 0xec, 0x98, 0xfa, 0x2e, 0x08,
        0x77, 0x13, 0xd0, 0x38, 0xe6, 0x21, 0x28, 0x45,
        0x6c, 0x0c, 0xe9, 0x34, 0xcf, 0x66, 0x54, 0xbe,
        0xb1, 0x5c, 0x95, 0xfd, 0x78, 0x4f, 0xf8, 0x7e,
        0xaa, 0x43, 0xac, 0xf1, 0x51, 0x08, 0x84, 0x85,
        0x54, 0x3c, 0x32, 0x25, 0x2f, 0xd3, 0x82, 0xc8,
        0x0d, 0x61, 0xe3, 0xe0, 0x95, 0x11, 0xa5, 0x64,
        0x99, 0x23, 0x0c, 0xca, 0x99, 0xa3, 0xb5, 0xd3,
        0xdd, 0x50, 0x7c, 0xc9, 0xb7, 0x29, 0xac, 0xc0
    };

    // unsigned int i;

    for(i = 0; i < 8; i++)
    {
        state[i] = state_init[i];
    }

    for(i = 0; i < 16; i++)
    {
        key[i] = key_init[i];
        S0[i] = S0_init[i];
        S1[i] = S1_init[i];
    }

    for(i = 0; i < 96; i++)
    {
        RC[i] = RC_init[i];
    }

    RunEncryptionKeySchedule(key, roundKeys);

    Encrypt(state, roundKeys);

    for(i = 0; i < 8; i++)
    {
        printf("%x ", state[i]);
    }
    
    return 0;
    // RETURN(0);
}
