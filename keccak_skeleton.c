#include <stddef.h>
#include <stdint.h>

#define NROUNDS 24
#define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))


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
static const uint64_t KeccakF_RoundConstants[NROUNDS] = {
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

/* Keccak rho rotation offsets r[x][y]  (x = 열, y = 행)
 *
 *          y=0   y=1   y=2   y=3   y=4
 *   x=0     0    36     3    41    18
 *   x=1     1    44    10    45     2
 *   x=2    62     6    43    15    61
 *   x=3    28    55    25    21    56
 *   x=4    27    20    39     8    14
 *
 * 필요하면 아래 표를 그대로 쓰거나, 자신의 state 레이아웃에 맞게 재배열해서
 * 쓰세요. 상수 테이블 없이 r[x][y] = ((t+1)(t+2)/2) mod 64 점화식으로
 * 생성해도 됩니다.
 */
static const unsigned int KeccakF_RhoOffsets[5][5] = {
    {  0, 36,  3, 41, 18 },
    {  1, 44, 10, 45,  2 },
    { 62,  6, 43, 15, 61 },
    { 28, 55, 25, 21, 56 },
    { 27, 20, 39,  8, 14 }
};

/*************************************************
 * Name:        theta
 *
 * Description: Keccak-f theta step
 *
 *              C[x]    = A[x,0] ^ A[x,1] ^ A[x,2] ^ A[x,3] ^ A[x,4]
 *              D[x]    = C[x-1] ^ ROL(C[x+1], 1)      (인덱스는 mod 5)
 *              A[x,y] ^= D[x]
 **************************************************/
static void theta(uint64_t *state) {
    /* TODO */
    (void)state;
}

/*************************************************
 * Name:        rho
 *
 * Description: Keccak-f rho step — lane 단위 좌회전
 *
 *              A[x,y] = ROL(A[x,y], r[x][y])
 **************************************************/
static void rho(uint64_t *state) {
    /* TODO */
    (void)state;
}

/*************************************************
 * Name:        pi
 *
 * Description: Keccak-f pi step — lane 위치 재배치
 *
 *              B[y, 2*x + 3*y] = A[x,y]               (인덱스는 mod 5)
 **************************************************/
static void pi(uint64_t *state) {
    /* TODO */
    (void)state;
}

/*************************************************
 * Name:        chi
 *
 * Description: Keccak-f chi step — 행 단위 비선형 변환
 *
 *              A[x,y] = A[x,y] ^ ((~A[x+1,y]) & A[x+2,y])   (인덱스는 mod 5)
 **************************************************/
static void chi(uint64_t *state) {
    /* TODO */
    (void)state;
}

/*************************************************
 * Name:        iota
 *
 * Description: Keccak-f iota step — 라운드 상수 주입
 *
 *              A[0,0] ^= KeccakF_RoundConstants[round]
 *
 **************************************************/
static void iota(uint64_t *state, unsigned int round) {
    /* TODO */
    (void)state;
    (void)round;
}

/*************************************************
 * Name:        KeccakF1600_StatePermute
 *
 * Description: The Keccak F1600 Permutation
 *              한 라운드 = iota(chi(pi(rho(theta(A)))))  를 24회 반복
 *
 * Arguments:   - uint64_t *state: pointer to input/output Keccak state
 *                                 (25 lanes, lane(x,y) == state[5*y + x])
 **************************************************/
static void KeccakF1600_StatePermute(uint64_t *state) {
    /* TODO: round = 0 .. NROUNDS-1 에 대해 theta/rho/pi/chi/iota 순으로 적용 */
    (void)state;
}


static void keccak_absorb(uint64_t *s, uint32_t r, const uint8_t *m,
                          size_t mlen, uint8_t p) {
    /* TODO
     *  1) state 25개 lane 을 0 으로 초기화
     *  2) mlen >= r 인 동안: 앞 r 바이트를 load64 로 읽어 s[0 .. r/8-1] 에 XOR,
     *     permute, m += r / mlen -= r
     *  3) 남은 mlen(< r) 바이트를 크기 r 의 0-초기화 임시 버퍼에 복사한 뒤
     *     pad10*1 적용:  t[mlen] = p;   t[r-1] |= 0x80;
     *     그 결과를 s 에 XOR (여기서는 permute 하지 않음 — squeeze 가 먼저 함)
     */
    (void)s;
    (void)r;
    (void)m;
    (void)mlen;
    (void)p;
}


static void keccak_squeezeblocks(uint8_t *h, size_t nblocks,
                                 uint64_t *s, uint32_t r) {
    /* TODO: nblocks 번 반복 — permute 한 뒤 s[0 .. r/8-1] 을 store64 로
     *       h 에 기록하고 h += r */
    (void)h;
    (void)nblocks;
    (void)s;
    (void)r;
}
