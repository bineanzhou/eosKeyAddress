/*
**  (c)maester.xyz
**  EOSIO keys and address CLang lib
**  No dependence except clib. Can be used on linux, windows, Android or iOS
*/


#include "ecdsa.h"
#include "bignum256.h"
#include "hash.h"
#include "sha256.h"
#include "ripemd160.h"



static const char b58digits_ordered[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

bool b58enc(char *b58, size_t *b58sz, const void *data, size_t binsz)
{
	const uint8_t *bin = data;
	int carry;
	size_t i, j, high, zcount = 0;
	size_t size;
	
	while (zcount < binsz && !bin[zcount])
		++zcount;
	
	size = (binsz - zcount) * 138 / 100 + 1;
	uint8_t buf[size];
	memset(buf, 0, size);
	
	for (i = zcount, high = size - 1; i < binsz; ++i, high = j)
	{
		for (carry = bin[i], j = size - 1; (j > high) || carry; --j)
		{
			carry += 256 * buf[j];
			buf[j] = (uint8_t)(carry % 58);
			carry /= 58;
		}
	}
	
	for (j = 0; j < size && !buf[j]; ++j);
	
	if (*b58sz <= zcount + size - j)
	{
		*b58sz = zcount + size - j + 1;
		return false;
	}
	
	if (zcount)
		memset(b58, '1', zcount);
	for (i = zcount; j < size; ++i, ++j)
		b58[i] = b58digits_ordered[buf[j]];
	b58[i] = '\0';
	*b58sz = i + 1;
	
	return true;
}


void eosPrivate2Address(const char privIn[64], char * privStr, char *addrStr)
{
    HashState hs; 
    uint8_t priv[32];
    uint8_t x[32];
    uint8_t y[32];
    uint8_t pub33[33];
    uint8_t hash[32];
    uint8_t pub37[37];
    uint8_t pub37Rev[37];
    uint8_t priv37[37];
    uint8_t priv37Rev[37];
    int i;

    bigFromHexString(privIn, priv);
    ecPubkey(priv, x, y);

    if(y[0] % 2)
        pub33[32] = 0x03;
    else 
        pub33[32] = 0x02;
    
    memcpy(pub33, x, 32);

    ripemd160Begin(&hs);
    for(i=0; i<33; i++)
        ripemd160WriteByte(&hs, pub33[32-i]);
    ripemd160Finish(&hs);
    writeHashToByteArray(hash, &hs, true);

    for(i=0; i<4; i++)
    {
        pub37[i] = hash[3-i];
    }
    memcpy(pub37+4, pub33, 33);

    for(i=36; i>=0; i--)
        pub37Rev[i] = pub37[36-i];
    size_t len = 256;
    b58enc(addrStr, &len, pub37Rev, 37);
    addrStr[len] = 0;

//////////////////////////////////////////////
// Priv-WIF

    memcpy(priv37+4, priv, 32);
    priv37[36] = 0x80;


    sha256Begin(&hs);
    for(i=0; i<33; i++)
        sha256WriteByte(&hs, priv37[36-i]);
    sha256FinishDouble(&hs);    
    writeHashToByteArray(hash, &hs, true);

    for(i=0; i<4; i++)
    {
        priv37[i] = hash[3-i];
    }

    for(i=36; i>=0; i--)
        priv37Rev[i] = priv37[36-i];
    len = 256;
    b58enc(privStr, &len, priv37Rev, 37);
    privStr[len] = 0;
}
