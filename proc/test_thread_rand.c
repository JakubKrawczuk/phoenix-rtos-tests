/*
 * Phoenix-RTOS
 *
 * phoenix-rtos-tests: random lifespan threads test
 *
 * Copyright 2021 Phoenix Systems
 * Author: Mateusz Niewiadomski
 *
 * This file is part of Phoenix-RTOS.
 *
 * %LICENSE%
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/threads.h>

#include "unity_fixture.h"


typedef struct {
	unsigned int time;
	unsigned int id;
} thread_params_t;


typedef struct {
	thread_params_t params;
	char stack[512] __attribute__((aligned(8))); /* stack needs to be aligned to 8 */
} thread_self_t;


typedef struct {
	unsigned int fin;
	handle_t fin_lock;
	thread_self_t threads[10]; /* set threads amount here */
} threads_common_t;


static threads_common_t threads_common;


static void test_thread(void *arg)
{
	thread_params_t *params = (thread_params_t *)arg;
	usleep(params->time * 1000);

	mutexLock(threads_common.fin_lock);
	threads_common.fin++;
	mutexUnlock(threads_common.fin_lock);

	endthread();
}


TEST_GROUP(threads_rand);


TEST_SETUP(threads_rand)
{
}


TEST_TEAR_DOWN(threads_rand)
{
}


TEST(threads_rand, test_1)
{
	int cthread, err, i, joined = 0;

	cthread = sizeof(threads_common.threads) / sizeof(threads_common.threads[0]);
	err = mutexCreate(&(threads_common.fin_lock));
	TEST_ASSERT_EQUAL_INT(err, EOK);
	threads_common.fin = 0;

	srand(7);
	/* assign threads parameters */
	for (i = 0; i < cthread; i++) {
		threads_common.threads[i].params.id = i;
		threads_common.threads[i].params.time = 100 + rand() % 200; /* assign thread lifespan in milliseconds */
		beginthread(test_thread, 6, &(threads_common.threads[i].stack), sizeof(threads_common.threads[i].stack), &(threads_common.threads[i].params));
	}

	/* all threads should end in under one second */
	for (i = 0; i < cthread; i++) {
		threadJoin(0);
		joined++;
	}

	resourceDestroy(threads_common.fin_lock);

	TEST_ASSERT_EQUAL_INT(cthread, threads_common.fin);
	TEST_ASSERT_EQUAL_INT(cthread, joined);
}


TEST_GROUP_RUNNER(threads_rand)
{
	RUN_TEST_CASE(threads_rand, test_1);
}


void runner(void)
{
	RUN_TEST_GROUP(threads_rand);
}


int main(int argc, char *argv[])
{
	UnityMain(argc, (const char **)argv, runner);
	return 0;
}
