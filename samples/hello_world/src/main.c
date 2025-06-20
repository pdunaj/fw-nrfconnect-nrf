/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <hal/nrf_cache.h>
#include <zephyr/kernel.h>
#include <zephyr/cache.h>

#define PROFILE_CACHE 1
#if PROFILE_CACHE
static uint32_t dhits;
static uint32_t dmisses;
static uint32_t ihits;
static uint32_t imisses;
#endif
#define CACHE_INVALIDATE 1

static void cache_profiling_init(void)
{
#if PROFILE_CACHE
	nrf_cache_profiling_set(NRF_DCACHE, 1);
	nrf_cache_profiling_set(NRF_ICACHE, 1);
#endif
}

static void cache_profiling_clear(void)
{
#if PROFILE_CACHE
	nrf_cache_profiling_counters_clear(NRF_DCACHE);
	nrf_cache_profiling_counters_clear(NRF_ICACHE);
#endif
}

static void cache_stats_update(void)
{	
#if PROFILE_CACHE
	dhits = nrf_cache_data_hit_counter_get(NRF_DCACHE, 0);
	dmisses = nrf_cache_data_miss_counter_get(NRF_DCACHE, 0);
	ihits = nrf_cache_data_hit_counter_get(NRF_ICACHE, 0);
	imisses = nrf_cache_data_miss_counter_get(NRF_ICACHE, 0);
#endif
}

static void cache_stats_print(void)
{
#if PROFILE_CACHE
	printf("\tData cache hits: %u\n", dhits);
	printf("\tData cache misses: %u\n", dmisses);
	printf("\tInstr cache hits: %u\n", ihits);
	printf("\tInstr cache misses: %u\n", imisses);
#endif
}

static void access_time(unsigned int *mem)
{
	size_t pos = 0;

#if CACHE_INVALIDATE
	sys_cache_data_flush_and_invd_all();
	sys_cache_instr_flush_and_invd_all();
	__sync_synchronize();
#endif

	int64_t start = k_uptime_get();
	cache_profiling_clear();
	for (size_t i = 0; i < 100*1000*1000; i++) {
		volatile register unsigned int temp = mem[pos];
		//ARG_UNUSED(temp);
		compiler_barrier();
	}
	cache_stats_update();
	int64_t stop = k_uptime_get();

	printf("time = %lld ms\n", (stop - start));
	cache_stats_print();
}

static void skip_access_time(unsigned char *mem, size_t max, size_t step)
{
	size_t pos = 0;

#if CACHE_INVALIDATE
	sys_cache_data_flush_and_invd_all();
	sys_cache_instr_flush_and_invd_all();
	__sync_synchronize();
#endif
	
	int64_t start = k_uptime_get();
	cache_profiling_clear();
	for (size_t i = 0; i < 100*1000*1000; i++) {
		pos = (pos + step) % max;
		volatile register unsigned int temp = mem[pos];
		//ARG_UNUSED(temp);
		compiler_barrier();
	}
	cache_stats_update();
	int64_t stop = k_uptime_get();

	printf("time = %lld ms\n", (stop - start));
	cache_stats_print();
}

int main(void)
{
	static unsigned char gram[196*1024]; // 196 KiB

	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);
	cache_profiling_init();

	// ram2:   0x2f890000
	// ram3:   0x2fc00000
	// mram:   0x0e100000
	// mram:   0x0e0f4000
	// apptcm: 0x22000000;
	printf("=== SINGLE ACCESS TIME ===\n");
	printf("\n");

	printf("=== gram ===\n");
	printf("gram: %p\n", gram);
	access_time((void *)gram);
	printf("\n");

	printf("=== app tcm ===\n");
	printf("app tcm: 0x22000000\n");
	access_time((void *)0x22000000);
	printf("\n");

	printf("=== ram2 ===\n");
	printf("ram2: 0x2f890000\n");
	access_time((void *)0x2f890000);
	printf("\n");

	printf("=== ram3 ===\n");
	printf("ram3: 0x2fc00000\n");
	access_time((void *)0x2fc00000);
	printf("\n");

	printf("=== mram ===\n");
	printf("mram: 0x0e100000\n");
	access_time((void *)0x0e100000);
	printf("\n");

	printf("\n");
	printf("=== SKIP ACCESS TIME ===\n");
	printf("\n");

	printf("=== app tcm ===\n");
	printf("app tcm: skip 0: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 0);
	printf("app tcm: skip 1: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 1);
	printf("app tcm: skip 13: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 13);
	printf("app tcm: skip 47: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 47);
	printf("app tcm: skip 64: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 64);
	printf("app tcm: skip 1717: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 1717);
	printf("\n");

	printf("=== gram ===\n");
	printf("gram: skip 0: %p\n", gram);
	skip_access_time((void *)gram, 4096, 0);
	printf("gram: skip 1: %p\n", gram);
	skip_access_time((void *)gram, 4096, 1);
	printf("gram: skip 13: %p\n", gram);
	skip_access_time((void *)gram, 4096, 13);
	printf("gram: skip 47: %p\n", gram);
	skip_access_time((void *)gram, 4096, 47);
	printf("gram: skip 1717: %p\n", gram);
	skip_access_time((void *)gram, 4096, 1717);
	printf("gram large: skip 1: %p\n", gram);
	skip_access_time((void *)gram, 40960, 1);
	printf("gram large: skip 1717: %p\n", gram);
	skip_access_time((void *)gram, 40960, 1717);
	printf("gram large: skip 9137: %p\n", gram);
	skip_access_time((void *)gram, 40960, 9137);
	printf("gram huge: skip 1: %p\n", gram);
	skip_access_time((void *)gram, sizeof(gram), 1);
	printf("gram huge: skip 1717: %p\n", gram);
	skip_access_time((void *)gram, sizeof(gram), 1717);
	printf("gram huge: skip 9137: %p\n", gram);
	skip_access_time((void *)gram, sizeof(gram), 9137);
	printf("\n");

	printf("=== ram2 ===\n");
	printf("ram2: skip 0: 0x2f890000\n");
	skip_access_time((void *)0x2f890000, 4096, 0);
	printf("ram2: skip 1: 0x2f890000\n");
	skip_access_time((void *)0x2f890000, 4096, 1);
	printf("ram2: skip 47: 0x2f890000\n");
	skip_access_time((void *)0x2f890000, 4096, 47);
	printf("\n");

	printf("=== ram3 ===\n");
	printf("ram3: skip 0: 0x2fc00000\n");
	skip_access_time((void *)0x2fc00000, 4096, 0);
	printf("ram3: skip 1: 0x2fc00000\n");
	skip_access_time((void *)0x2fc00000, 4096, 1);
	printf("ram3: skip 47: 0x2fc00000\n");
	skip_access_time((void *)0x2fc00000, 4096, 47);
	printf("\n");

	printf("=== mram ===\n");
	printf("mram: skip 0: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 4096, 0);
	printf("mram: skip 1: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 4096, 1);
	printf("mram: skip 47: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 4096, 47);	
	printf("mram: skip 1717: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 4096, 1717);
	printf("mram large: skip 1717: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 40960, 1717);
	printf("mram large: skip 9137: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 40960, 9137);
	printf("mram huge: skip 1: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, sizeof(gram), 1);
	printf("mram huge: skip 64: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, sizeof(gram), 64);
	printf("mram huge: skip 1717: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, sizeof(gram), 1717);
	printf("mram huge: skip 9137: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, sizeof(gram), 9137);
	printf("\n");

	return 0;
}
