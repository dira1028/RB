/* ============================================================================
 *  keccak.h - Keccak / SHA-3 스펀지 구조 구현 과제
 *  ------------------------------------------------------------------------
 *  이 헤더는 수정하지 마세요. (인터페이스 고정)
 * ========================================================================== */
#ifndef KECCAK_H
#define KECCAK_H

#include <stdint.h>

/* ---------------------------------------------------------------- 상수 정의 */
#define KECCAK_SPONGE_BIT   1600            /* 상태 크기 b = r + c        */
#define KECCAK_STATE_WORDS  25              /* 1600 / 64                  */
#define KECCAK_ROUNDS       24              /* 12 + 2*l, l = 6            */

#define SHA3_OK              0
#define SHA3_PARAMETER_ERROR (-1)

/* 도메인 분리 패딩 시작 바이트 */
#define PAD_SHA3    0x06                    /* SHA3-224/256/384/512       */
#define PAD_SHAKE   0x1F                    /* SHAKE128 / SHAKE256        */
#define PAD_KECCAK  0x01                    /* 원본 Keccak                */

/* (rate, capacity) 비트 조합 */
#define SHA3_224_RATE   1152
#define SHA3_256_RATE   1088
#define SHA3_384_RATE    832
#define SHA3_512_RATE    576
#define SHAKE128_RATE   1344
#define SHAKE256_RATE   1088

/* ------------------------------------------------------------- 유틸 매크로 */
/* 64비트 좌회전. n == 0 일 때 (x >> 64) 는 정의되지 않은 동작이므로 주의할 것. */
#define ROL64(x, n) (((n) == 0) ? (x) : ((((uint64_t)(x)) << (n)) | (((uint64_t)(x)) >> (64 - (n)))))

/* 상태 배열 인덱싱: A[x, y] = state[x + 5*y],  0 <= x, y < 5 */
#define IDX(x, y) ((x) + 5 * (y))

/* --------------------------------------------------------- 전역 상태 (외부) */
extern uint64_t keccak_state[KECCAK_STATE_WORDS];
extern int end_offset;
extern int is_squeezing;

/* --------------------------------------------------------------- 내부 순열 */
void theta(uint64_t *state);
void rho(uint64_t *state);
void pi(uint64_t *state);
void chi(uint64_t *state);
void iota(uint64_t *state, int round);
void keccakf(uint64_t *state);

/* --------------------------------------------------------------- 스펀지 API */
void keccak_init(void);
int  keccak_absorb(uint8_t *input, int inLen, int rate, int capacity);
int  keccak_squeeze(uint8_t *output, int outLen, int rate, int capacity,
                    uint8_t pad_byte);

#endif /* KECCAK_H */
