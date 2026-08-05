/* ============================================================================
 *  main.c - Keccak / SHA-3 구현 검증용 테스트 하네스
 *  ------------------------------------------------------------------------
 *  빌드 : make
 *  실행 : ./keccak_test
 * ========================================================================== */

#include <stdio.h>
#include <string.h>
#include "keccak.h"

static int g_pass = 0;
static int g_fail = 0;

/* --------------------------------------------------------------------------
 *  한 번에 해싱하는 편의 래퍼
 * ------------------------------------------------------------------------ */
static int hash_once(int rate, uint8_t pad,
                     const uint8_t *msg, int msgLen,
                     uint8_t *out, int outLen)
{
    int capacity = KECCAK_SPONGE_BIT - rate;
    int rc;

    keccak_init();

    rc = keccak_absorb((uint8_t *)msg, msgLen, rate, capacity);
    if (rc != SHA3_OK) return rc;

    return keccak_squeeze(out, outLen, rate, capacity, pad);
}

/* --------------------------------------------------------------------------
 *  16진 문자열 비교 및 결과 출력
 * ------------------------------------------------------------------------ */
static void check(const char *name, const uint8_t *got, int len,
                  const char *expected_hex)
{
    char buf[257];
    int i;

    if (len > 128) len = 128;
    for (i = 0; i < len; i++)
        sprintf(buf + 2 * i, "%02x", got[i]);
    buf[2 * len] = '\0';

    if (strcmp(buf, expected_hex) == 0)
    {
        printf("  [ PASS ] %s\n", name);
        g_pass++;
    }
    else
    {
        printf("  [ FAIL ] %s\n", name);
        printf("           expected: %s\n", expected_hex);
        printf("           got     : %s\n", buf);
        g_fail++;
    }
}

/* ========================================================================= */
int main(void)
{
    uint8_t out[64];
    uint8_t msg200[200];
    int capacity;

    memset(msg200, 'a', sizeof(msg200));

    printf("=== Keccak / SHA-3 Test Vectors ===\n\n");

    /* ---- SHA3-224 ---- */
    printf("SHA3-224\n");
    hash_once(SHA3_224_RATE, PAD_SHA3, (const uint8_t *)"", 0, out, 28);
    check("\"\" (empty)", out, 28,
          "6b4e03423667dbb73b6e15454f0eb1abd4597f9a1b078e3f5b5a6bc7");

    /* ---- SHA3-256 ---- */
    printf("SHA3-256\n");
    hash_once(SHA3_256_RATE, PAD_SHA3, (const uint8_t *)"", 0, out, 32);
    check("\"\" (empty)", out, 32,
          "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a");

    hash_once(SHA3_256_RATE, PAD_SHA3, (const uint8_t *)"abc", 3, out, 32);
    check("\"abc\"", out, 32,
          "3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431532");

    /* rate(136바이트) 를 넘는 다중 블록 입력 */
    hash_once(SHA3_256_RATE, PAD_SHA3, msg200, 200, out, 32);
    check("'a' x 200 (multi-block)", out, 32,
          "cce34485baf2bf2aca99b94833892a4f52896d3d153f7b840cc4f9fe695f1387");

    /* ---- SHA3-384 ---- */
    printf("SHA3-384\n");
    hash_once(SHA3_384_RATE, PAD_SHA3, (const uint8_t *)"", 0, out, 48);
    check("\"\" (empty)", out, 48,
          "0c63a75b845e4f7d01107d852e4c2485c51a50aaaa94fc61995e71bbee983a2a"
          "c3713831264adb47fb6bd1e058d5f004");

    /* ---- SHA3-512 ---- */
    printf("SHA3-512\n");
    hash_once(SHA3_512_RATE, PAD_SHA3, (const uint8_t *)"", 0, out, 64);
    check("\"\" (empty)", out, 64,
          "a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a6"
          "15b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26");

    hash_once(SHA3_512_RATE, PAD_SHA3, (const uint8_t *)"abc", 3, out, 64);
    check("\"abc\"", out, 64,
          "b751850b1a57168a5693cd924b6b096e08f621827444f70d884f5d0240d2712e"
          "10e116e9192af3c91a7ec57647e3934057340b4cf408d5a56592f8274eec53f0");

    /* ---- SHAKE ---- */
    printf("SHAKE128 / SHAKE256\n");
    hash_once(SHAKE128_RATE, PAD_SHAKE, (const uint8_t *)"", 0, out, 32);
    check("SHAKE128(\"\", 32B)", out, 32,
          "7f9c2ba4e88f827d616045507605853ed73b8093f6efbc88eb1a6eacfa66ef26");

    hash_once(SHAKE256_RATE, PAD_SHAKE, (const uint8_t *)"", 0, out, 32);
    check("SHAKE256(\"\", 32B)", out, 32,
          "46b9dd2b0ba88d13233b3feb743eeb243fcd52ea62b81b82b50c27646ed5762f");

    /* SHAKE128 은 rate 가 168바이트이므로 64바이트 출력은 단일 블록.
     * 여러 번에 나누어 squeeze 해도 같은 결과가 나와야 한다.               */
    {
        uint8_t part[64];
        capacity = KECCAK_SPONGE_BIT - SHAKE128_RATE;

        keccak_init();
        keccak_absorb((uint8_t *)"abc", 3, SHAKE128_RATE, capacity);
        keccak_squeeze(part,      10, SHAKE128_RATE, capacity, PAD_SHAKE);
        keccak_squeeze(part + 10, 54, SHAKE128_RATE, capacity, PAD_SHAKE);
        check("SHAKE128(\"abc\", 64B) split squeeze", part, 64,
              "5881092dd818bf5cf8a3ddb793fbcba74097d5c526a6d35f97b83351940f2cc8"
              "44c50af32acd3f2cdd066568706f509bc1bdde58295dae3f891a9a0fca578378");
    }

    /* ---- 스트리밍 absorb (여러 번 나누어 입력) ---- */
    printf("Streaming absorb\n");
    {
        capacity = KECCAK_SPONGE_BIT - SHA3_256_RATE;

        keccak_init();
        keccak_absorb(msg200,       1, SHA3_256_RATE, capacity);
        keccak_absorb(msg200 +   1, 70, SHA3_256_RATE, capacity);
        keccak_absorb(msg200 +  71, 100, SHA3_256_RATE, capacity);
        keccak_absorb(msg200 + 171, 29, SHA3_256_RATE, capacity);
        keccak_squeeze(out, 32, SHA3_256_RATE, capacity, PAD_SHA3);
        check("SHA3-256('a' x 200) in 4 chunks", out, 32,
              "cce34485baf2bf2aca99b94833892a4f52896d3d153f7b840cc4f9fe695f1387");
    }

    /* ---- 파라미터 검증 ---- */
    printf("Parameter validation\n");
    {
        int rc;
        keccak_init();
        rc = keccak_absorb((uint8_t *)"x", 1, 1088, 1088);   /* r+c != 1600 */
        if (rc == SHA3_PARAMETER_ERROR) { printf("  [ PASS ] r+c != 1600 rejected\n"); g_pass++; }
        else                            { printf("  [ FAIL ] r+c != 1600 rejected\n"); g_fail++; }

        keccak_init();
        keccak_squeeze(out, 32, SHA3_256_RATE, 512, PAD_SHA3);
        rc = keccak_absorb((uint8_t *)"x", 1, SHA3_256_RATE, 512);
        if (rc == SHA3_PARAMETER_ERROR) { printf("  [ PASS ] absorb-after-squeeze rejected\n"); g_pass++; }
        else                            { printf("  [ FAIL ] absorb-after-squeeze rejected\n"); g_fail++; }
    }

    printf("\n===================================\n");
    printf(" PASS: %d   FAIL: %d\n", g_pass, g_fail);
    printf("===================================\n");

    return (g_fail == 0) ? 0 : 1;
}
