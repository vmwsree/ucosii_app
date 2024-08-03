/*
 * Copyright (C) 2021 Intel Corporation
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <reent.h>
#include <limits.h>
#include "includes.h"

#define NO_TIMEOUT 			0
#define MY_TASK_LIST_EN 	0 // 1
#define MY_QUEUE_LIST_EN 	1


/* Definition of priorities */
#define MUTEX_PRIORITY      1
#if (MY_TASK_LIST_EN > 0u)
#define TASK1_PRIORITY      2
#define TASK2_PRIORITY      3
#define TASK3_PRIORITY      4
#define TASK4_PRIORITY      5
#endif

#define TASK7_PRIORITY      8
#define TASK8_PRIORITY      9

#define NUM_PRINTS_PER_TASK 150

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       1024 // 2048
#if (MY_TASK_LIST_EN > 0u)
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
#if (TASK3_PRIORITY > 0u)
OS_STK    task3_stk[TASK_STACKSIZE];
OS_STK    task4_stk[TASK_STACKSIZE];
#endif
#endif

OS_EVENT  *g_mutex;

#if (MY_QUEUE_LIST_EN > 0u)
#define MAX_N_MSGS	14

OS_EVENT	*g_msgqueue;
void *MessageStorage[MAX_N_MSGS];
OS_EVENT	*g_msgqueue2;
void *MessageStorage2[MAX_N_MSGS + MAX_N_MSGS];
OS_STK    task5_stk[TASK_STACKSIZE];
OS_STK    task6_stk[TASK_STACKSIZE];
#define 	TASK5_PRIORITY      6
#define 	TASK6_PRIORITY      7
#if (TASK7_PRIORITY > 0u)
	OS_STK    task7_stk[TASK_STACKSIZE];
#endif
#if (TASK8_PRIORITY > 0u)
	OS_STK    task8_stk[TASK_STACKSIZE];
#endif
#endif // MY_QUEUE_LIST_EN

#if (MY_TASK_LIST_EN > 0u)
void task1(void* pdata)
{
	INT8U err;
	int i = 0;
	int ms_slept = *((int *)pdata);
	while (1)
	{
		OSMutexPend(g_mutex, NO_TIMEOUT, &err);
		if (i < NUM_PRINTS_PER_TASK) {
			printf("Hello from task1: %d\n", i++);
		}
		OSMutexPost(g_mutex);
		OSTimeDlyHMSM(0, 0, 0, ms_slept);
	}
}

void task2(void* pdata)
{
	INT8U err;
	int i = 0;
	int ms_slept = *((int *)pdata);
	while (1)
	{
		OSMutexPend(g_mutex, NO_TIMEOUT, &err);
		if (i < NUM_PRINTS_PER_TASK) {
			printf("Hello from task2: %d\n", i++);
		}
		OSMutexPost(g_mutex);
	//	OSTaskSuspend(TASK2_PRIORITY);
		OSTimeDlyHMSM(0, 0, 0, ms_slept);
	}
}
#endif

#if (TASK3_PRIORITY > 0u)
void task3(void* pdata)
{
	INT8U err;
	int i = 0;
	int ms_slept = *((int *)pdata);
	while (1)
	{
		OSMutexPend(g_mutex, NO_TIMEOUT, &err);
		if (i < NUM_PRINTS_PER_TASK) {
			printf("Hello from task3: %d\n", i++);
		}
		OSMutexPost(g_mutex);
		OSTimeDlyHMSM(0, 0, 0, ms_slept);
	}
}


void task4(void* pdata)
{
	INT8U err;
	int i = 0;
	int ms_slept = *((int *)pdata);
	while (1)
	{
		OSMutexPend(g_mutex, NO_TIMEOUT, &err);
		if (i < NUM_PRINTS_PER_TASK) {
			printf("Hello from task4: %d\n", i++);
		}
		OSMutexPost(g_mutex);
		OSTimeDlyHMSM(0, 0, 0, ms_slept);
	}
}
#endif // 0

#if (MY_QUEUE_LIST_EN > 0u)
void task5(void* pdata)
{
	//char msg[20] = "q_task 5";
	int i = 1000;

	pdata = pdata;
	while (1)
	{
		OSQPost(g_msgqueue, (void*)&i);
		printf("Task5 Post: %d \n", i++);
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

void task6(void* pdata)
{
	INT8U err;
	int i = 0;
	int *data;

	pdata = pdata;
	while (1)
	{
		data = (int*)OSQPend(g_msgqueue, 0, &err);
		if (i < NUM_PRINTS_PER_TASK) {
			printf("Hello from task6: %d\n", i++);
		}
		printf("Task6 pend data: %d \n", *data);

		OSTimeDlyHMSM(0, 0, 0, 400);
	}
}

#if (TASK7_PRIORITY > 0u)
void task7(void* pdata)
{
	INT8U err;
	int *data;

	pdata = pdata;
	while (1)
	{
		data = (int*)OSQPend(g_msgqueue, 0, &err);
		printf("TASK 7: %d\n", *data);

		OSTimeDlyHMSM(0, 0, 0, 500);
	}
}
#endif

#if (TASK8_PRIORITY > 0u)
void task8(void* pdata)
{
	INT8U err;
	int *data;

	pdata = pdata;
	while (1)
	{
		data = (int*)OSQPend(g_msgqueue, 0, &err);
		printf("TASK 8: %d\n", (int)*data);
		OSTimeDlyHMSM(0, 0, 0, 400);
	}
}
#endif // TASK8_PRIORITY
#endif

void SetTaskName(void) {
	INT8U err;
#if (MY_TASK_LIST_EN > 0u)
	OSTaskNameSet(TASK1_PRIORITY, (INT8U *)(void *)"My Task_1", &err);
	OSTaskNameSet(TASK2_PRIORITY, (INT8U *)"My Task_2", &err);
	OSTaskNameSet(TASK3_PRIORITY, (INT8U *)(void *)"My Task_3", &err);
	OSTaskNameSet(TASK4_PRIORITY, (INT8U *)"My Task_4", &err);
#endif
#if (MY_QUEUE_LIST_EN > 0u)
	OSTaskNameSet(TASK5_PRIORITY, (INT8U *)(void *)"My Q_Task_5", &err);
	OSTaskNameSet(TASK6_PRIORITY, (INT8U *)"My Q_Task_6", &err);
	OSTaskNameSet(TASK7_PRIORITY, (INT8U *)(void *)"My Q_Task_7", &err);
	OSTaskNameSet(TASK8_PRIORITY, (INT8U *)"My Q_Task_8", &err);
#endif
}


/* The main function creates tasks and starts multi-tasking */
int main(void)
{
	printf("Hello from main...\n");
#if (MY_TASK_LIST_EN > 0u)
	printf("Task1 -- TOS: %p, BOS: %p\n", &task1_stk[TASK_STACKSIZE-1], &task1_stk[0]);
	printf("Task2 -- TOS: %p, BOS: %p\n", &task2_stk[TASK_STACKSIZE-1], &task2_stk[0]);
#if (TASK3_PRIORITY > 0u)
	printf("Task3 -- TOS: %p, BOS: %p\n", &task3_stk[TASK_STACKSIZE-1], &task3_stk[0]);
	printf("Task4 -- TOS: %p, BOS: %p\n", &task4_stk[TASK_STACKSIZE-1], &task4_stk[0]);
#endif
#endif
#if (MY_QUEUE_LIST_EN > 0u)
	printf("Task5 -- TOS: %p, BOS: %p\n", &task5_stk[TASK_STACKSIZE-1], &task5_stk[0]);
	printf("Task6 -- TOS: %p, BOS: %p\n", &task6_stk[TASK_STACKSIZE-1], &task6_stk[0]);
#endif /* MY_QUEUE_LIST_EN */
	printf("Stat -- TOS: %p, BOS: %p\n", &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1u], &OSTaskStatStk[0]);
	printf("Idle -- TOS: %p, BOS: %p\n", &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1u], &OSTaskIdleStk[0]);

	INT8U err;

#if (MY_QUEUE_LIST_EN > 0u)
	g_msgqueue = OSQCreate((void**)&MessageStorage[0], MAX_N_MSGS);
	if ((void*)0 == g_msgqueue) {
		/* Msg Queue creation failed */
		printf(" Msg Queue creation failed  ***\n");
	}
#if (OS_EVENT_NAME_EN > 0u)
	OSEventNameSet(g_msgqueue, (INT8U*)"First Msg Queue Event", &err);
#endif
#endif // MY_QUEUE_LIST_EN

	g_mutex = OSMutexCreate(MUTEX_PRIORITY, &err);
#if (OS_EVENT_NAME_EN > 0u)
	OSEventNameSet(g_mutex, (INT8U*)"My Mutex Event", &err);
#endif

#if (MY_QUEUE_LIST_EN > 0u)
	g_msgqueue2 = OSQCreate((void**)&MessageStorage2[0], MAX_N_MSGS + MAX_N_MSGS);
	if ((void*)0 == g_msgqueue2) {
		/* Msg Queue creation failed */
		printf(" Msg Queue creation failed   g_msgqueue2 ***\n");
	}
#if (OS_EVENT_NAME_EN > 0u)
	OSEventNameSet(g_msgqueue2, (INT8U*)(void *)"My Second MsgQ Event", &err);
#endif
	int qtask_data = 0;
#endif // MY_QUEUE_LIST_EN

#if (MY_TASK_LIST_EN > 0u)
	int t1_ms_slept = 600;
	int t2_ms_slept = 990;
#if (TASK3_PRIORITY > 0u)
	int t3_ms_slept = 350;
	int t4_ms_slept = 400;
#endif
#endif

#if (MY_TASK_LIST_EN > 0u)
	err = OSTaskCreateExt(task1,
			&t1_ms_slept,
			(void *)&task1_stk[TASK_STACKSIZE-1],
			TASK1_PRIORITY,
			TASK1_PRIORITY,
			task1_stk,
			TASK_STACKSIZE,
			NULL,
			0);
    if (OS_ERR_NONE == err) {
    	printf(" ***** OSTaskCreateExt OK ***** \n");
    }

	OSTaskCreateExt(task2,
			&t2_ms_slept,
			(void *)&task2_stk[TASK_STACKSIZE-1],
			TASK2_PRIORITY,
			TASK2_PRIORITY,
			task2_stk,
			TASK_STACKSIZE,
			NULL,
			0);

#if (TASK3_PRIORITY > 0u)
    err = OSTaskCreateExt(task3,
			&t3_ms_slept,
			(void *)&task3_stk[TASK_STACKSIZE-1],
			TASK3_PRIORITY,
			TASK3_PRIORITY,
			task3_stk,
			TASK_STACKSIZE,
			NULL,
			0);

    if (OS_ERR_NONE != err) {
    	printf(" ***** OSTaskCreateExt ERROR ***** \n");
    }
#if OS_TASK_NAME_EN > 0u
//    OSTaskNameSet(TASK3_PRIORITY, (INT8U *)"My Task 3", &err);
#endif

	OSTaskCreateExt(task4,
			&t4_ms_slept,
			(void *)&task4_stk[TASK_STACKSIZE-1],
			TASK4_PRIORITY,
			TASK4_PRIORITY,
			task4_stk,
			TASK_STACKSIZE,
			NULL,
			0);
    if (OS_ERR_NONE != err) {
    	printf(" ***** OSTaskCreateExt ERROR ***** \n");
    }
#endif
#endif // 0

#if (MY_QUEUE_LIST_EN > 0u)
	err = OSTaskCreateExt(task5,
			&qtask_data,
			(void *)&task5_stk[TASK_STACKSIZE-1],
			TASK5_PRIORITY,
			TASK5_PRIORITY,
			task5_stk,
			TASK_STACKSIZE,
			NULL,
			0);

	err = OSTaskCreateExt(task6,
			&qtask_data,
			(void *)&task6_stk[TASK_STACKSIZE-1],
			TASK6_PRIORITY,
			TASK6_PRIORITY,
			task6_stk,
			TASK_STACKSIZE,
			NULL,
			0);
    if (OS_ERR_NONE != err) {
    	printf(" ***** OSTaskCreateExt ERROR ***** \n");
    }

#if (TASK7_PRIORITY > 0u)
	err = OSTaskCreateExt(task7,
			&qtask_data,
			(void *)&task7_stk[TASK_STACKSIZE-1],
			TASK7_PRIORITY,
			TASK7_PRIORITY,
			task7_stk,
			TASK_STACKSIZE,
			NULL,
			0);
    if (OS_ERR_NONE != err) {
    	printf(" ***** OSTaskCreateExt ERROR ***** \n");
    }
#endif

#if (TASK8_PRIORITY > 0u)
	err = OSTaskCreateExt(task8,
			&qtask_data,
			(void *)&task8_stk[TASK_STACKSIZE-1],
			TASK8_PRIORITY,
			TASK8_PRIORITY,
			task8_stk,
			TASK_STACKSIZE,
			NULL,
			0);
    if (OS_ERR_NONE != err) {
    	printf(" ***** OSTaskCreateExt ERROR ***** \n");
    }
#endif
#endif // MY_QUEUE_LIST_EN

    SetTaskName();

	OSStart();
	return 0;
}
