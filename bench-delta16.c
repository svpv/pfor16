#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#if defined(__i386__) || defined(__x86_64__)
#include <x86intrin.h>
#define rdtsc() __rdtsc()
#else
static inline uint64_t rdtsc(void)
{
    uint64_t t;
#if defined(__aarch64__)
    asm volatile("mrs %0, cntvct_el0" : "=r"(t));
#elif defined(__powerpc64__)
    asm volatile("mfspr %0, 268" : "=r"(t));
#else
#error "rdtsc not supported"
#endif
    return t;
}
#endif

#define R32K UINT64_C(6364136223846793005)
static uint64_t rand32state = R32K;

static inline uint32_t rand32(void)
{
    uint32_t ret = rand32state >> 32;
    rand32state *= R32K;
    return ret;
}

void delta16enc(uint16_t *v, size_t n);
void delta16dec(uint16_t *v, size_t n);

#ifndef NLOG
#define NLOG 10
#endif

int main()
{
    uint16_t v[1<<NLOG];
    for (int i = 0; i < (1<<NLOG); i++)
	v[i] = rand32();
    uint64_t t0 = rdtsc();
    for (int i = 0; i < (1<<(32-NLOG)); i++)
	delta16enc(v, (1<<NLOG));
    uint64_t t1 = rdtsc();
    for (int i = 0; i < (1<<(32-NLOG)); i++)
	delta16dec(v, (1<<NLOG));
    uint64_t t2 = rdtsc();
    printf("delta16enc: %.2f cycles per integer\n"
	   "delta16dec: %.2f cycles per integer\n",
	   (t1 - t0) / (double) UINT32_MAX,
	   (t2 - t1) / (double) UINT32_MAX);
    rand32state = R32K;
    for (int i = 0; i < (1<<NLOG); i++)
	assert(v[i] == (uint16_t) rand32());
    return 0;
}
