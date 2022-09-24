#include "helpers.h"
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t __attribute__((noinline))
count_pairs(uint8_t *data, uint64_t size, uint8_t target) {
  uint64_t total = 0;
  uint32_t last_bit = 0;
  __m256i compare = _mm256_set1_epi8(target);
  for (uint64_t i = 0; i < size * 2; i += 32) {
    uint32_t block = _mm256_movemask_epi8(
        _mm256_cmpeq_epi8(_mm256_load_si256((__m256i *)(data + i)), compare));
    total += __builtin_popcount(block & (block >> 1U));
    if (last_bit) {
      total += last_bit & block;
    }
    last_bit = block >> 31U;
  }
  return total;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("specify the byte to look for and how log of the number of bytes to "
           "generate\n");
  }
  uint8_t target = atoi(argv[1]) & 0xFF;
  uint64_t logn = atol(argv[2]);
  uint64_t n = 1UL << logn;
  printf("target byte is %u, creating %lu bytes of data\n", target, 2 * n);
  uint16_t *data = get_random_data(n);
  uint64_t start = get_usecs();
  uint64_t pair_count = count_pairs((uint8_t *)data, n, target);
  uint64_t end = get_usecs();
  printf("took %lu milleseconds, and found %lu aligned pairs\n",
         (end - start) / 1000, pair_count);
  return 0;
}