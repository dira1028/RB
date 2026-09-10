#include <stddef.h>
#include <stdint.h>

//ROL 매크로 추가
#define ROL(a,offsett) (((a) << (offsett)) ^ ((a) >> (64 - (offsett))))


static uint64_t load64(const uint8_t *x) {
    uint64_t r = 0;
        for (size_t i = 0; i < 8; ++i) {
            r |= (uint64_t)x[i] << 8 * i;
        }
        return r;
}


static void store64(uint8_t *x, uint64_t u) {
    size_t i;
    for (i = 0; i < 8; ++i) {
        x[i] = (uint8_t)(u >> 8 * i);
    }
}


/* Keccak round constants (iota 단계에서 사용) */
static const uint64_t KeccakF_RoundConstants[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL,
    0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL,
    0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL,
    0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL,
    0x0000000080000001ULL, 0x8000000080008008ULL
};

static const unsigned int KeccakF_RhoOffsets[5][5] = {
    {  0, 36,  3, 41, 18 },
    {  1, 44, 10, 45,  2 },
    { 62,  6, 43, 15, 61 },
    { 28, 55, 25, 21, 56 },
    { 27, 20, 39,  8, 14 }
};

uint64_t State[5][5];
uint64_t S[25];
static void theta(uint64_t *state) {
    uint64_t C[5], D[5];
    for(int i=0;i<5;i++){
        C[i] = state[i] ^ state[i+5] ^ state[i+10] ^ state[i+15] ^ state[i+20];
    }
    for(int i=0;i<5;i++){
        D[i] = C[(i+4)%5] ^ ROL(C[(i+1)%5], 1); 
    }

    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            state[i + 5*j] ^= D[i];
        }
    }
}


static void rho_pi(uint64_t *state) {
    unsigned long long temp = 0;
}


static void chi(uint64_t *state) {
    unsigned long long temp[25];

    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            temp[i + 5*j] = state[i + 5*j] ^ ((~state[(i+1)%5 + 5*j]) & state[(i+2)%5 + 5*j]);
        }
    }
}


static void iota(uint64_t *state, unsigned int round) {
    /* TODO */
}


static void KeccakF1600_StatePermute(uint64_t *state) {
    /* TODO: round = 0 .. NROUNDS-1 에 대해 theta/rho/pi/chi/iota 순으로 적용 */
}




static void keccak_absorb(uint64_t *s, uint32_t r, const uint8_t *m,
                          size_t mlen, uint8_t p) {
    while(mlen >= r){
        //for문으로 2) 코드추가
    }
    /* TODO
     *  1) state 25개 lane 을 0 으로 초기화
     *  2) mlen >= r 인 동안: 앞 r 바이트를 load64 로 읽어 s[0 .. r/8-1] 에 XOR,
     *     permute, m += r / mlen -= r
     *  3) 임시버퍼 uint8_t t[200];
     *     메시지가 끝나는 지점에 패딩 추가(메시지는 Byte단위로 들어옴);
     *     마지막에 패딩의 끝 추가하기 
     */
}


static void keccak_squeezeblocks(uint8_t *h, size_t nblocks,
                                 uint64_t *s, uint32_t r) {
    /* TODO: permutation후 추출*/
}
