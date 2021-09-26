#pragma once
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

uint64_t get_usecs() {
  struct timeval st;
  gettimeofday(&st, NULL);
  return st.tv_sec * 1000000 + st.tv_usec;
}

uint16_t *get_random_data(uint64_t size) {
#ifdef SEED
  srand(SEED);
#else
  srand(0);
#endif
  uint16_t *data = (uint16_t *)memalign(32, size * sizeof(*data));
  for (uint64_t i = 0; i < size; i++) {
    data[i] = rand();
  }
  return data;
}

uint16_t load16(void *loc) {
  uint16_t data;
  memcpy(&data, loc, sizeof(data));
  return data;
}