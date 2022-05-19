/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         HAMC-SHA1 algorithm
 *
 *  \note          
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2019 by Vinfast Trading And Production Llc                                         All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vinfast Trading And Production Llc.
 *                Vinfast Trading And Production Llc grants to you only those rights as set out in the license 
 *                conditions.
 *                All other rights remain with Vinfast Trading And Production Llc.
 *  \endverbatim
 */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Doan Manh                     DM            Vinfast Trading And Production Llc
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2020-10-29  DM      N/A              Fix all misra 2012 errors
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "HMACSHA1.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
/* Number of bytes in a word    */
#define SHA_WORD_SIZE 4u
/* HMAC input K length Bytes	*/
#define K_LENGTH 8u

/* Length of SHA1 Algorithm		*/
/* 20-Byte Don't change it     	*/
#define SHA_DIGEST_SIZE 20u
/* Size of digest in word       */
#define SHA_DIGEST_WORD_SIZE (SHA_DIGEST_SIZE / SHA_WORD_SIZE)
/* Block of SHA1 Algorithm		*/
/* 64-Byte Don't change it     	*/
#define SHA_BLOCK_SIZE 64u
/* Size of block in word       */
#define SHA_BLOCK_WORD_SIZE (SHA_BLOCK_SIZE / SHA_WORD_SIZE)

/* Size of counter in byte      */
#define SHA_COUNTER_SIZE 8u
/* Size of counter in word      */
#define SHA_COUNTER_WORD_SIZE (SHA_COUNTER_SIZE / SHA_WORD_SIZE)

#define SHA_K00_19 0x5A827999u
#define SHA_K20_39 0x6ED9EBA1u
#define SHA_K40_59 0x8F1BBCDCu
#define SHA_K60_79 0xCA62C1D6u

#define SHA_HASH_0 0x67452301u
#define SHA_HASH_1 0xEFCDAB89u
#define SHA_HASH_2 0x98BADCFEu
#define SHA_HASH_3 0x10325476u
#define SHA_HASH_4 0xC3D2E1F0u

#define SHA_I_PAD 0x36u
#define SHA_O_PAD 0x5Cu

#define SHA_MASK_HIGH  0xFF00FF00u
#define SHA_MASK_LOW   0x00FF00FFu

#define rol(value, bits)    (((value) << (bits)) | ((value) >> (32u - (bits))))

#define blk0(i)             (block->l[(i)] = (rol(block->l[(i)], 24u) & SHA_MASK_HIGH) | (rol(block->l[(i)], 8u) & SHA_MASK_LOW))
#define blk(i)              (block->l[(i)&15u] = rol(block->l[((i) + 13u) & 15u] ^ block->l[((i) + 8u) & 15u] ^ block->l[((i) + 2u) & 15u] ^ block->l[(i)&15u], 1u))

#define R0(v, w, x, y, z, i)                                                        \
    blk0(i);                                                                        \
    (z) += (((w) & ((x) ^ (y))) ^ (y)) + block->l[(i)] + SHA_K00_19 + rol((v), 5u); \
    (w) = rol((w), 30u)
#define R1(v, w, x, y, z, i)                                                            \
    blk(i);                                                                             \
    (z) += (((w) & ((x) ^ (y))) ^ (y)) + block->l[(i)&15u] + SHA_K00_19 + rol((v), 5u); \
    (w) = rol((w), 30u)
#define R2(v, w, x, y, z, i)                                                  \
    blk(i);                                                                   \
    (z) += ((w) ^ (x) ^ (y)) + block->l[(i)&15u] + SHA_K20_39 + rol((v), 5u); \
    (w) = rol((w), 30u)
#define R3(v, w, x, y, z, i)                                                                    \
    blk(i);                                                                                     \
    (z) += ((((w) | (x)) & (y)) | ((w) & (x))) + block->l[(i)&15u] + SHA_K40_59 + rol((v), 5u); \
    (w) = rol((w), 30u)
#define R4(v, w, x, y, z, i)                                                  \
    blk(i);                                                                   \
    (z) += ((w) ^ (x) ^ (y)) + block->l[(i)&15u] + SHA_K60_79 + rol((v), 5u); \
    (w) = rol((w), 30u)

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
static __u8 workspace[SHA_BLOCK_SIZE];

/***********************************************************************************************************************
 *  TYPES
 **********************************************************************************************************************/
typedef struct
{
    __u32 state[SHA_DIGEST_WORD_SIZE];  /* constant of SHA1 */
    __u32 count[SHA_COUNTER_WORD_SIZE]; /* counter */
    __u8 buffer[SHA_BLOCK_SIZE];        /* 512-bit message blocks */
} SHA1_CTX;

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
static void *my_memcpy(void *dest, const void *src, __u32 count);
static void *my_memset(void *ptr, __u8 c, __u32 count);

static void SHA1Transform(__u32 state[SHA_DIGEST_WORD_SIZE], const __u8 buffer[SHA_BLOCK_SIZE]);
static void SHA1Reset(SHA1_CTX *context);
static void SHA1Input(SHA1_CTX *context, __u8 *data, __u32 len);
static void SHA1Result(__u8 digest[SHA_DIGEST_SIZE], SHA1_CTX *context);
static void SHA1Truncate(const __u8 *d1, __u8 *d2, __u32 len);

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  my_memcpy
 **********************************************************************************************************************/
/*! \brief         Memory copy
 *  \details       None
 *  \param[out]    dest: Pointer to destination array
 *  \param[in]     src: Pointer to source bytes array
 *  \param[in]     count: Number of bytes to copy
 *  \return        None
 **********************************************************************************************************************/
static void *my_memcpy(
    void *dest,
    const void *src,
    __u32 count)
{
    __u8 *d = (__u8 *)dest;
    const __u8 *s = (const __u8 *)src;
    while (count != 0u)
    {
        *d = *s;
        d++;
        s++;
        count--;
    }
    
    return dest;
}

/***********************************************************************************************************************
 *  my_memset
 **********************************************************************************************************************/
/*! \brief         Set all bytes in memory to a value
 *  \details       None
 *  \param[in,out] ptr: Pointer to memory to be set
 *  \param[in]     c: Byte value to be set
 *  \param[in]     count: Number of bytes to be set
 *  \return        None
 **********************************************************************************************************************/
static void *my_memset(
    void *ptr,
    __u8 c,
    __u32 count)
{
    __u8 *p = (__u8 *)ptr;
    while (count != 0u)
    {
        *p = c;
        p++;
        count--;
    }
    return ptr;
}

/***********************************************************************************************************************
 *  SHA1Transform
 **********************************************************************************************************************/
/*! \brief         SHA transform function 
 *  \details       None
 *  \param[in,out] state: Store state of hash process
 *  \param[in]     buffer: Data to be hashed
 *  \return        None
 **********************************************************************************************************************/
static void SHA1Transform(
    __u32 state[SHA_DIGEST_WORD_SIZE], /* constants used in SHA1 */
    const __u8 buffer[SHA_BLOCK_SIZE]        /* data to be hashed */
)
{
    __u32 a = 0u;
    __u32 b = 0u;
    __u32 c = 0u;
    __u32 d = 0u;
    __u32 e = 0u;
    typedef struct
    {
        __u32 l[SHA_BLOCK_WORD_SIZE];
    } CHAR64LONG16;

    CHAR64LONG16 *block = (CHAR64LONG16 *)workspace;

    (void)my_memcpy(block, buffer, SHA_BLOCK_SIZE);
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    /* 80 steps used to calculate SHA1 digest */
    R0(a, b, c, d, e, 0u);
    R0(e, a, b, c, d, 1u);
    R0(d, e, a, b, c, 2u);
    R0(c, d, e, a, b, 3u);
    R0(b, c, d, e, a, 4u);
    R0(a, b, c, d, e, 5u);
    R0(e, a, b, c, d, 6u);
    R0(d, e, a, b, c, 7u);
    R0(c, d, e, a, b, 8u);
    R0(b, c, d, e, a, 9u);
    R0(a, b, c, d, e, 10u);
    R0(e, a, b, c, d, 11u);
    R0(d, e, a, b, c, 12u);
    R0(c, d, e, a, b, 13u);
    R0(b, c, d, e, a, 14u);
    R0(a, b, c, d, e, 15u);
    R1(e, a, b, c, d, 16u);
    R1(d, e, a, b, c, 17u);
    R1(c, d, e, a, b, 18u);
    R1(b, c, d, e, a, 19u);
    R2(a, b, c, d, e, 20u);
    R2(e, a, b, c, d, 21u);
    R2(d, e, a, b, c, 22u);
    R2(c, d, e, a, b, 23u);
    R2(b, c, d, e, a, 24u);
    R2(a, b, c, d, e, 25u);
    R2(e, a, b, c, d, 26u);
    R2(d, e, a, b, c, 27u);
    R2(c, d, e, a, b, 28u);
    R2(b, c, d, e, a, 29u);
    R2(a, b, c, d, e, 30u);
    R2(e, a, b, c, d, 31u);
    R2(d, e, a, b, c, 32u);
    R2(c, d, e, a, b, 33u);
    R2(b, c, d, e, a, 34u);
    R2(a, b, c, d, e, 35u);
    R2(e, a, b, c, d, 36u);
    R2(d, e, a, b, c, 37u);
    R2(c, d, e, a, b, 38u);
    R2(b, c, d, e, a, 39u);
    R3(a, b, c, d, e, 40u);
    R3(e, a, b, c, d, 41u);
    R3(d, e, a, b, c, 42u);
    R3(c, d, e, a, b, 43u);
    R3(b, c, d, e, a, 44u);
    R3(a, b, c, d, e, 45u);
    R3(e, a, b, c, d, 46u);
    R3(d, e, a, b, c, 47u);
    R3(c, d, e, a, b, 48u);
    R3(b, c, d, e, a, 49u);
    R3(a, b, c, d, e, 50u);
    R3(e, a, b, c, d, 51u);
    R3(d, e, a, b, c, 52u);
    R3(c, d, e, a, b, 53u);
    R3(b, c, d, e, a, 54u);
    R3(a, b, c, d, e, 55u);
    R3(e, a, b, c, d, 56u);
    R3(d, e, a, b, c, 57u);
    R3(c, d, e, a, b, 58u);
    R3(b, c, d, e, a, 59u);
    R4(a, b, c, d, e, 60u);
    R4(e, a, b, c, d, 61u);
    R4(d, e, a, b, c, 62u);
    R4(c, d, e, a, b, 63u);
    R4(b, c, d, e, a, 64u);
    R4(a, b, c, d, e, 65u);
    R4(e, a, b, c, d, 66u);
    R4(d, e, a, b, c, 67u);
    R4(c, d, e, a, b, 68u);
    R4(b, c, d, e, a, 69u);
    R4(a, b, c, d, e, 70u);
    R4(e, a, b, c, d, 71u);
    R4(d, e, a, b, c, 72u);
    R4(c, d, e, a, b, 73u);
    R4(b, c, d, e, a, 74u);
    R4(a, b, c, d, e, 75u);
    R4(e, a, b, c, d, 76u);
    R4(d, e, a, b, c, 77u);
    R4(c, d, e, a, b, 78u);
    R4(b, c, d, e, a, 79u);
    /* Return the SHA1 digest */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

/***********************************************************************************************************************
 *  SHA1Reset
 **********************************************************************************************************************/
/*! \brief         Reset the state of hash process
 *  \details       None
 *  \param[in,out] context: Current context of hash process
 *  \param[in]     None
 *  \return        None
 **********************************************************************************************************************/
static void SHA1Reset(
    SHA1_CTX *context /* SHA1 structure need to reset */
)
{
    context->state[0] = SHA_HASH_0;
    context->state[1] = SHA_HASH_1;
    context->state[2] = SHA_HASH_2;
    context->state[3] = SHA_HASH_3;
    context->state[4] = SHA_HASH_4;
    context->count[0] = 0u;
    context->count[1] = 0u;
}

/***********************************************************************************************************************
 *  SHA1Input
 **********************************************************************************************************************/
/*! \brief         Process the input data for hash function
 *  \details       None
 *  \param[in,out] context: Current context of hash process
 *  \param[in]     data: Data block input
 *  \param[in]     len: Lenght of data block
 *  \return        None
 **********************************************************************************************************************/
static void SHA1Input(
    SHA1_CTX *context, /* result returned from here */
    __u8 *data,        /* data to be processed */
    __u32 len          /* data length  */
)
{
    __u32 i = 0u;
    __u32 j = 0u;

    j = context->count[0];
    context->count[0] += (len << 3u);
    if (context->count[0] < j)
    {
        context->count[1]++;
    }
    context->count[1] += (len >> 29u);
    j = (j >> 3u) & 63u;
    if ((j + len) > 63u)
    {
        i = (__u32)(64u - j);
        (void)my_memcpy(&context->buffer[j], data, i);
        SHA1Transform(context->state, context->buffer);
        for (; (i + 63u) < len; i += 64u)
        {
            SHA1Transform(context->state, &data[i]);
        }
        j = 0u;
    }
    else
    {
        i = 0u;
    }
    (void)my_memcpy(&context->buffer[j], &data[i], len - i);
    
}

/***********************************************************************************************************************
 *  SHA1Result
 **********************************************************************************************************************/
/*! \brief         Last step of sha hmac
 *  \details       None
 *  \param[in,out] context: Current context of hash process
 *  \param[in,out] digest: Calculated digest
 *  \return        None
 **********************************************************************************************************************/
static void SHA1Result(
    __u8 digest[SHA_DIGEST_SIZE], /* SHA1 digest  */
    SHA1_CTX *context            /* message need to pad and calculate  */
)
{
    __u32 i = 0u;
    __u8 finalcount[8];
    __u8 str1[1] ={128u};
    __u8 str2[1] = {0u};

    /* Pad the message */
    for (i = 0u; i < 8u; i++)
    {
        finalcount[i] = (__u8)((context->count[((i >= 4u) ? 0u : 1u)] >> ((__u8)(3u - (i & 3u)) * 8u)) & 255u);
    }
    SHA1Input(context, str1, 1u);
    while ((context->count[0] & 504u) != 448u)
    {
        SHA1Input(context, str2, 1u);
    }
    SHA1Input(context, (__u8 *)finalcount, 8u);
    for (i = 0u; i < SHA_DIGEST_SIZE; i++)
    {
        digest[i] = (__u8)((context->state[i >> 2u] >> ((__u8)(3u - (i & 3u)) * 8u)) & 255u);
    }
    /* Wipe variables */
    (void)my_memset(context->buffer, 0x0u, SHA_BLOCK_SIZE);
    (void)my_memset(context->state, 0x0u, SHA_DIGEST_SIZE);
    (void)my_memset(context->count, 0x0u, SHA_COUNTER_SIZE);
    (void)my_memset(&finalcount, 0x0u, 8u);
    /* Calculate the SHA1 digest */
    SHA1Transform(context->state, context->buffer);
}

/***********************************************************************************************************************
 *  SHA1Truncate
 **********************************************************************************************************************/
/*! \brief         None
 *  \details       None
 *  \param[in,out] None
 *  \param[in]     None
 *  \return        None
 **********************************************************************************************************************/
static void SHA1Truncate(
    const __u8 *d1, /* data to be truncated */
    __u8 *d2, /* truncated data */
    __u32 len /* length in bytes to keep */
)
{
    __u32 i = 0u;
    for (i = 0u; i < len; i++)
    {
        d2[i] = d1[i];
    }
}

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  HMACSHA1
 **********************************************************************************************************************/
/*! \brief         None
 *  \details       None
 *  \param[in,out] None
 *  \param[in]     None
 *  \return        None
 **********************************************************************************************************************/
void HMACSHA1(
    const __u8 *k,  /* data of HMAC-K */
    __u8 *d,  /* data to be Hash */
    __u8 d_len,  /* data to be Hash length*/
    __u8 *out, /* digest output buffer */
    __u8 out_len /* digest output buffer length*/
)
{
    SHA1_CTX ictx;
    SHA1_CTX octx;
    __u8 isha[SHA_DIGEST_SIZE];
    __u8 osha[SHA_DIGEST_SIZE];
    __u8 buf[SHA_BLOCK_SIZE];
    __u32 i = 0u;

    /*************** Perform inner SHA1 ******************/
    SHA1Reset(&ictx);

    for (i = 0u; i < K_LENGTH; ++i) /* Pad k with ipad */
    {
        buf[i] = k[i] ^ SHA_I_PAD;
    }

    for (i = K_LENGTH; i < SHA_BLOCK_SIZE; ++i)
    {
        buf[i] = SHA_I_PAD;
    }

    SHA1Input(&ictx, buf, SHA_BLOCK_SIZE); /* start with inner pad */
    SHA1Input(&ictx, d, d_len);     /* then text of datagram */
    SHA1Result(isha, &ictx);              /* finish up 1st pass */
    /*************** Perform outer SHA1 ******************/
    SHA1Reset(&octx); /* innt context for 1st pass*/

    for (i = 0u; i < K_LENGTH; ++i) /* Pad K with opad */
    {
        buf[i] = k[i] ^ SHA_O_PAD;
    }

    for (i = K_LENGTH; i < SHA_BLOCK_SIZE; ++i)
    {
        buf[i] = SHA_O_PAD;
    }

    SHA1Input(&octx, buf, SHA_BLOCK_SIZE);   /* start with outer pad */
    SHA1Input(&octx, isha, SHA_DIGEST_SIZE); /* then results of 1st hash */
    SHA1Result(osha, &octx);                /* finish up 2nd pass */

    /*************** Truncate the results ****************/
    SHA1Truncate(osha, out, out_len);
}

/***********************************************************************************************************************
 *  END OF FILE: HMACSHA1.C
 **********************************************************************************************************************/
