/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>

static void access_time(unsigned int *mem)
{
	size_t pos = 0;

	int64_t start = k_uptime_get();
	for (size_t i = 0; i < 100*1000*1000; i++) {
		volatile register unsigned int temp = mem[pos];
		//ARG_UNUSED(temp);
		compiler_barrier();
	}
	int64_t stop = k_uptime_get();

	printf("time = %lld ms\n", (stop - start));
}

static void skip_access_time(unsigned int *mem, size_t max, size_t step)
{
	size_t pos = 0;
	
	int64_t start = k_uptime_get();
	for (size_t i = 0; i < 100*1000*1000; i++) {
		pos = (pos + step) % max;
		volatile register unsigned int temp = mem[pos];
		//ARG_UNUSED(temp);
		compiler_barrier();
	}
	int64_t stop = k_uptime_get();

	printf("time = %lld ms\n", (stop - start));
}

int main(void)
{
	static unsigned int gram[10240];

	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

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
	printf("app tcm: skip 1: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 1);
	printf("app tcm: skip 13: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 13);
	printf("app tcm: skip 47: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 47);
	printf("app tcm: skip 1717: 0x22000000\n");
	skip_access_time((void *)0x22000000, 4096, 1717);
	printf("\n");

	printf("=== gram ===\n");
	printf("gram: skip 1: %p\n", gram);
	skip_access_time((void *)gram, 4096, 1);
	printf("gram: skip 13: %p\n", gram);
	skip_access_time((void *)gram, 4096, 13);
	printf("gram: skip 47: %p\n", gram);
	skip_access_time((void *)gram, 4096, 47);
	printf("gram: skip 1717: %p\n", gram);
	skip_access_time((void *)gram, 4096, 1717);
	printf("gram: skip 9137: %p\n", gram);
	skip_access_time((void *)gram, 40960, 9137);
	printf("\n");

	printf("=== ram2 ===\n");
	printf("ram2: skip 1: 0x2f890000\n");
	skip_access_time((void *)0x2f890000, 4096, 1);
	printf("ram2: skip 47: 0x2f890000\n");
	skip_access_time((void *)0x2f890000, 4096, 47);
	printf("\n");

	printf("=== ram3 ===\n");
	printf("ram3: skip 1: 0x2fc00000\n");
	skip_access_time((void *)0x2fc00000, 4096, 1);
	printf("ram3: skip 47: 0x2fc00000\n");
	skip_access_time((void *)0x2fc00000, 4096, 47);
	printf("\n");

	printf("=== mram ===\n");
	printf("mram: skip 1: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 4096, 1);
	printf("mram: skip 47: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 4096, 47);	
	printf("mram: skip 1717: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 4096, 1717);
	printf("mram: skip 9137: 0x0e100000\n");
	skip_access_time((void *)0x0e100000, 40960, 9137);
	
	printf("\n");

	return 0;
}
