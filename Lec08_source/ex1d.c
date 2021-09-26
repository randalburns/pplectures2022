#include "helpers.h"
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t __attribute__((noinline))
count_pairs(uint16_t *data, uint64_t size, uint8_t target) {
  data = __builtin_assume_aligned(data, 32);
  uint64_t total = 0;
  uint16_t check = target | (target << 8U);
  __m256i compare = _mm256_set1_epi16(check);
  for (uint64_t j = 0; j < size; j += 1024) {
    __m256i eq_vec = _mm256_setzero_si256();
    for (uint64_t i = 0; i < 1024; i += 64) {
      __m256i a = _mm256_cmpeq_epi16(
          _mm256_load_si256((__m256i *)(data + j + i)), compare);
      __m256i b = _mm256_cmpeq_epi16(
          _mm256_load_si256((__m256i *)(data + j + i)), compare);
      __m256i c = _mm256_cmpeq_epi16(
          _mm256_load_si256((__m256i *)(data + j + i)), compare);
      __m256i d = _mm256_cmpeq_epi16(
          _mm256_load_si256((__m256i *)(data + j + i)), compare);
      __m256i e = _mm256_add_epi16(a, b);
      __m256i f = _mm256_add_epi16(c, d);
      __m256i g = _mm256_add_epi16(e, f);
      eq_vec = _mm256_sub_epi16(eq_vec, g);
    }
    eq_vec = _mm256_hadd_epi16(eq_vec, eq_vec);
    eq_vec = _mm256_hadd_epi16(eq_vec, eq_vec);
    eq_vec = _mm256_hadd_epi16(eq_vec, eq_vec);
    total += _mm256_extract_epi16(eq_vec, 0);
    total += _mm256_extract_epi16(eq_vec, 8);
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
  if (n < 1024) {
    printf("n must be at least 1024\n");
    return 1;
  }
  printf("target byte is %u, creating %lu bytes of data\n", target, 2 * n);
  uint16_t *data = get_random_data(n);
  uint64_t start = get_usecs();
  uint64_t pair_count = count_pairs(data, n, target);
  uint64_t end = get_usecs();
  printf("took %lu milleseconds, and found %lu aligned pairs\n",
         (end - start) / 1000, pair_count);
  return 0;
}