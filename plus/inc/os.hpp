#ifndef OS_H
#define OS_H 

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"

#define MAX_NUM_OF_TASKS	10

typedef struct{
	TaskFunction_t 	function;	// void (*function)(void*);
	const char*     name;		// The name of this task;
	//size_t          stackSize;	// Allocate bytes for stack;
	void*			param;		// Parameters;
	UBaseType_t		priority;	//
	//StackType_t*	stack;		// Stack array;
	//StaticTask_t*	TCB;		// Task Control Block;
	TaskHandle_t 	handle;		// Task handle. Value will be returned when task is created successsfully;
}TaskPackage_t;

typedef enum{
	Task_1 = 0,
	Task_2,
	Task_3,
	Task_4,
	Task_5,
	Task_6,
	Task_7,
	Task_8,
	Task_9,
	Task_10,
}TaskIndex_t;

class OperatingSystem{
public:
	OperatingSystem(void){
		OperatingSystem::initTaskList();
	}

	static void start(void);

	static StackType_t  Idle_Task_Stack  [configMINIMAL_STACK_SIZE];			// 空闲任务堆栈
	static StackType_t  Timer_Task_Stack [configTIMER_TASK_STACK_DEPTH];		// 定时器任务堆栈 

	static StaticTask_t Idle_Task_TCB;			// 空闲任务控制块内存
	static StaticTask_t Timer_Task_TCB;			// 定时器任务控制块内存

	static TaskHandle_t AppTaskCreate_Handle;
	static StackType_t  AppTaskCreate_Stack[128];
	static StaticTask_t AppTaskCreate_TCB;

	static StackType_t  Task1_Stack [128];		// 定时器任务堆栈 
	static StaticTask_t Task1_TCB;				// 定时器任务控制块内存

	static StackType_t  Task2_Stack [64];		// 定时器任务堆栈 
	static StaticTask_t Task2_TCB;				// 定时器任务控制块内存


	static void assignTask(TaskIndex_t index,TaskFunction_t function,const char* name,UBaseType_t priority,void* param = NULL);
	
private:
	static TaskPackage_t task_list[MAX_NUM_OF_TASKS];

	static StackType_t   Task_Stack [MAX_NUM_OF_TASKS][128];		// 定时器任务堆栈 
	static StaticTask_t  Task_TCB[MAX_NUM_OF_TASKS];				// 定时器任务控制块内存

	static void initTasks(void);
	static void initTaskList(void);
};

#ifdef __cplusplus
extern "C"{
#endif

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize);

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, 
									StackType_t **ppxIdleTaskStackBuffer, 
									uint32_t *pulIdleTaskStackSize);

#ifdef __cplusplus
}
#endif

#endif		