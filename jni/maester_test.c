/*
**  (c)maester.xyz
**  EOSIO keys and address CLang lib
**  No dependence except clib. Can be used on linux, windows, Android or iOS
*/

#include <stdio.h>
#include "eoskeys.h"

int main(void)
{
    const char *priv = "618c79503a3d1476211c8be7876e94e8723596a1e0b0204c050229983c9bfebd";
    char privStr[256] = "PRIV";
    char addrStr[256] = "ADDR";

    eosPrivate2Address(priv, privStr, addrStr);

    printf("PRIV: %s\nPRIV_CODED: %s\nADDR: EOS%s\n", priv, privStr, addrStr);

    return 0;
}