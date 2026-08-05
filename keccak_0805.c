/* ============================================================================
 *  keccak.c - Keccak / SHA-3 스펀지 구조 구현 과제 (스켈레톤)
 *  ------------------------------------------------------------------------
 *  [TODO] 로 표시된 부분을 모두 채워 넣으세요.
 *  함수 시그니처와 keccak.h 는 수정하지 마세요.
 *
 *  ※ 상태 배열 표기 규약
 *     A[x, y] = keccak_state[x + 5*y]   (0 <= x, y < 5)
 *     매크로 IDX(x, y) 를 사용하면 편합니다.
 *
 *  ※ 바이트 순서
 *     상태를 바이트 배열로 볼 때는 리틀엔디언(little-endian)입니다.
 *     즉 (uint8_t*)keccak_state 로 캐스팅해서 접근하면 됩니다.
 *     (리틀엔디언 머신 가정. 빅엔디언 대응은 선택 과제)
 * ========================================================================== */

#include <string.h>
#include "keccak.h"

/* ---------------------------------------------------------------------------
 *  전역 상태
 * ------------------------------------------------------------------------- */

/* 64비트 25워드로 구성된 상태 배열 (1600비트) */
uint64_t keccak_state[KECCAK_STATE_WORDS] = { 0 };

/* 현재 블록에서 데이터가 채워진(또는 출력된) 바이트 위치 */
int end_offset = 0;

/* Squeeze 단계로 넘어갔는지 확인하기 위한 플래그 */
int is_squeezing = 0;

/* ---------------------------------------------------------------------------
 *  상수 테이블
 * ------------------------------------------------------------------------- */

/* rho_offsets*/
static const int rho_offsets[25] = {0,
};

/* iota 단계의 라운드 상수 RC[0..23] */
static const uint64_t round_constants[KECCAK_ROUNDS] = {
    0x0000000000000001ULL, 0x0000000000008082ULL,
    0x800000000000808AULL, 0x8000000080008000ULL,
    0x000000000000808BULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008AULL, 0x0000000000000088ULL,
    0x0000000080008009ULL, 0x000000008000000AULL,
    0x000000008000808BULL, 0x800000000000008BULL,
    0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800AULL, 0x800000008000000AULL,
    0x8000000080008081ULL, 0x8000000000008080ULL,
    0x0000000080000001ULL, 0x8000000080008008ULL
};

/* ===========================================================================
 *  [문제 1] theta
 *  ---------------------------------------------------------------------------
 *  C[x] = A[x,0] ^ A[x,1] ^ A[x,2] ^ A[x,3] ^ A[x,4]
 *  D[x] = C[(x+4) mod 5] ^ ROL64(C[(x+1) mod 5], 1)
 *  A[x,y] = A[x,y] ^ D[x]
 * ========================================================================= */
void theta(uint64_t *state)
{
    uint64_t temp_state[KECCAK_STATE_WORDS];
    uint64_t C[5];
    uint64_t D[5];
    int x, y;

    /* 1단계: 각 열 x 에 대하여 5개의 행 y 값을 모두 XOR 하여 C[x] 계산 */
    for (x = 0; x < 5; x++)
    {
        /* [TODO] C[x] 계산 */
    }

    /* 2단계: C 배열을 이용해 D[x] 계산
     * 힌트: (x-1) mod 5 는 음수가 되지 않도록 (x+4) % 5 로 처리한다. */
    for (x = 0; x < 5; x++)
    {
        /* [TODO] D[x] 계산 */
    }

    /* 3단계: 계산된 D[x] 를 상태 배열 전체에 반영 */
    for (y = 0; y < 5; y++)
    {
        for (x = 0; x < 5; x++)
        {
            /* [TODO] state[IDX(x, y)] 갱신 */
        }
    }

    (void)state; (void)C; (void)D;   /* 구현 후 이 줄은 삭제해도 됩니다 */
}

void keccakf(uint64_t *state)
{
    int round;

    for (round = 0; round < KECCAK_ROUNDS; round++)
    {
        theta(state);
        // rho(state);
        // pi(state);
        // chi(state);
        // iota(state, round);
    }

    (void)state; (void)round;
}

/* ===========================================================================
 *  keccak_init - 전역 상태 초기화 (제공됨)
 *  ---------------------------------------------------------------------------
 *  새로운 메시지를 해싱하기 전에 반드시 호출해야 한다.
 * ========================================================================= */
void keccak_init(void)
{
    memset(keccak_state, 0, sizeof(keccak_state));
    end_offset  = 0;
    is_squeezing = 0;
}

/* ===========================================================================
 *  [문제 7] keccak_absorb
 *  ---------------------------------------------------------------------------
 *  입력을 rate 바이트 단위 블록으로 나누어 상태에 XOR 하고,
 *  블록이 가득 찰 때마다 keccakf 를 수행한다.
 *  이 함수는 여러 번 나누어 호출될 수 있다 (스트리밍). 따라서 직전 호출에서
 *  남은 부분 블록 위치를 end_offset 으로 이어받아 처리해야 한다.
 * ========================================================================= */
int keccak_absorb(uint8_t *input, int inLen, int rate, int capacity)
{
    uint8_t *buf        = input;
    int      iLen       = inLen;
    int      rateInBytes = rate / 8;
    int      blockSize  = 0;
    int      i          = 0;

    /* 64비트 상태 배열을 바이트 단위로 접근하기 위한 포인터 */
    uint8_t *state_bytes = (uint8_t *)keccak_state;


    while (iLen > 0)
    {
        if ((end_offset != 0) && (end_offset < rateInBytes))
        {
            /* 직전 호출에서 채우다 만 부분 블록을 이어서 채우는 경우 */

            /* [TODO] blockSize 를 min(iLen + end_offset, rateInBytes) 로 계산 */

            /* [TODO] state_bytes[end_offset .. blockSize-1] 에
             *        buf[0 .. ] 를 XOR                                     */

            /* [TODO] buf 와 iLen 을 실제 소비한 바이트 수만큼 전진/감소 */
        }
        else
        {
            /* 블록 경계에서 새로 시작하는 경우 */

            /* [TODO] blockSize 를 min(iLen, rateInBytes) 로 계산 */

            /* [TODO] state_bytes[0 .. blockSize-1] 에 buf 를 XOR */

            /* [TODO] buf 와 iLen 갱신 */
        }

        /* [TODO] 블록이 가득 찼다면(blockSize == rateInBytes)
         *        keccakf 를 호출하고 blockSize 를 0 으로 되돌린다.         */

        end_offset = blockSize;

        break;  /* [TODO] 구현 완료 후 이 break 를 반드시 제거할 것 */
    }

    (void)i; (void)buf; (void)state_bytes;
    return SHA3_OK;
}

/* ===========================================================================
 *  [문제 8] keccak_squeeze
 *  ---------------------------------------------------------------------------
 *  최초 호출 시 패딩(pad10*1)을 적용하고 keccakf 를 수행한 뒤,
 *  rate 영역에서 outLen 바이트를 순차적으로 읽어 낸다.
 *  outLen 이 rate 보다 크면 rate 만큼 뽑을 때마다 keccakf 를 다시 수행한다.
 *
 *  pad_byte : PAD_SHA3(0x06) / PAD_SHAKE(0x1F) / PAD_KECCAK(0x01)
 * ========================================================================= */
int keccak_squeeze(uint8_t *output, int outLen, int rate, int capacity,
                   uint8_t pad_byte)
{
    uint8_t *out_buf     = output;
    int      iLen        = outLen;
    int      rateInBytes = rate / 8;

    /* 64비트 상태 배열을 바이트 단위로 읽기 위한 포인터 */
    uint8_t *state_bytes = (uint8_t *)keccak_state;

    int blockSize = 0;

    /* ---- 파라미터 검증 (제공됨) ---- */
    if ((rate + capacity) != KECCAK_SPONGE_BIT)
        return SHA3_PARAMETER_ERROR;

    if (((rate % 8) != 0) || (rate < 1))
        return SHA3_PARAMETER_ERROR;

    /* Squeeze 단계로 처음 진입하는 경우: 패딩(Padding) 수행 */
    if (!is_squeezing)
    {
        /* [TODO] end_offset 위치에 패딩 시작 바이트(pad_byte) XOR */

        /* [TODO] rate 영역의 마지막 바이트(rateInBytes - 1)에 0x80 XOR */

        /* [TODO] 패딩이 끝났으므로 내부 상태를 섞음 (keccakf 호출) */

        /* 출력(Squeeze)을 위해 오프셋 초기화 및 상태 변경 */
        end_offset  = 0;
        is_squeezing = 1;
    }

    /* 출력 버퍼(output)에 해시값 짜내기 */
    while (iLen > 0)
    {
        /* 남은 출력 길이와, 현재 블록에서 뽑아낼 수 있는 최대 길이 비교 */
        blockSize = ((iLen < (rateInBytes - end_offset))
                        ? iLen
                        : (rateInBytes - end_offset));

        /* [TODO] state_bytes + end_offset 에서 blockSize 바이트를
         *        out_buf 로 복사 (memcpy 또는 for 문)                      */

        out_buf    += blockSize;
        iLen       -= blockSize;
        end_offset += blockSize;

        /* [TODO] end_offset == rateInBytes 라면 keccakf 를 다시 호출하고
         *        end_offset 을 0 으로 초기화                               */
    }

    (void)state_bytes; (void)pad_byte;
    return SHA3_OK;
}
