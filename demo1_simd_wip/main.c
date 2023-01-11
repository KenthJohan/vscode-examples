#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include "flecs.h"


uint8_t ecs_log2(uint32_t v) {
    static const uint8_t log2table[32] = 
        {0, 9,  1,  10, 13, 21, 2,  29, 11, 14, 16, 18, 22, 25, 3, 30,
         8, 12, 20, 28, 15, 17, 24, 7,  19, 27, 23, 6,  26, 5,  4, 31};
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return log2table[(uint32_t)(v * 0x07C4ACDDU) >> 27];
}

uint8_t get_bucket_shift (int32_t bucket_count){return (uint8_t)(64u - ecs_log2((uint32_t)bucket_count));}


uint64_t index_from_key(uint64_t key, uint8_t shift_amount)
{
  //                4294967295
  return (int32_t)((11400714819323198485ull * key) >> shift_amount);
}




void keys_to_indices(uint64_t const keys[4], uint64_t indices[4], uint8_t shift)
{
  __m256i m256_magic = _mm256_set1_epi64x(11400714819323198485ull);
  __m256i m256_keys = _mm256_load_epi64((void*)keys);
  __m256i m256_result = _mm256_srli_epi64(_mm256_mul_epu32(m256_magic, m256_keys), shift);
  _mm256_storeu_epi64(indices, m256_result);
}


typedef struct
{
  uint64_t key;
} bucket_t;

int main(int argc, char *argv[])
{
  uint8_t shift_amount = get_bucket_shift(16);
  /*

  */

  for(int i = 0; i < 100; ++i)
  {
    uint64_t keys[4] = {0};
    uint64_t index[4] = {i, i+100, i+200, i+300};
    test(keys, keys, shift_amount);
    printf("%ju %ju %ju %ju\n", keys[0], keys[1], keys[2], keys[3]);
  }
 
}