#include "os.hpp"
#include "system.hpp"
#include "fsl_common.h"

#include "fsl_debug_console.h"


//================================================================================================
TaskHandle_t  OperatingSystem::AppTaskCreate_Handle;
TaskPackage_t OperatingSystem::task_list[10];

StaticTask_t  OperatingSystem::Idle_Task_TCB;		// 空闲任务控制块内存
StaticTask_t  OperatingSystem::Timer_Task_TCB;		// 定时器任务控制块内存
StaticTask_t  OperatingSystem::AppTaskCreate_TCB;
StaticTask_t  OperatingSystem::Task1_TCB;
StaticTask_t  OperatingSystem::Task2_TCB;


StackType_t   OperatingSystem::Idle_Task_Stack      [configMINIMAL_STACK_SIZE];			// 空闲任务堆栈
StackType_t   OperatingSystem::Timer_Task_Stack     [configTIMER_TASK_STACK_DEPTH];		// 定时器任务堆栈
StackType_t   OperatingSystem::AppTaskCreate_Stack  [128];
StackType_t   OperatingSystem::Task1_Stack          [128];								// 任务1堆栈 
StackType_t   OperatingSystem::Task2_Stack          [64];								// 任务2堆栈 

StackType_t   OperatingSystem::Task_Stack 			[MAX_NUM_OF_TASKS][128];			// 任务总栈 
StaticTask_t  OperatingSystem::Task_TCB 			[MAX_NUM_OF_TASKS];					// 任务总控制模块
//================================================================================================

void OperatingSystem::initTaskList(void){
	size_t i = MAX_NUM_OF_TASKS;
	while(i--){
		task_list[i].function 	= NULL;
		task_list[i].name		= NULL;
		task_list[i].param		= NULL;
	} 
}


void OperatingSystem::assignTask(TaskIndex_t index,TaskFunction_t function,const char* name,UBaseType_t priority,void* param){
	assert(index < MAX_NUM_OF_TASKS);
	task_list[index].function 	= function;
	task_list[index].name		= name;
	task_list[index].priority	= priority;
	task_list[index].param		= param;
}


void OperatingSystem::initTasks(void){


	taskENTER_CRITICAL(); 


	for(size_t i=0; i<MAX_NUM_OF_TASKS; i++){
		if(task_list[i].function == NULL)
			continue;
		task_list[i].handle = xTaskCreateStatic(	(TaskFunction_t)task_list[i].function,
													(const char* )	task_list[i].name, 		//任务名称
													(uint32_t )		128, 					//任务堆栈大小
													(void* )		task_list[i].param, 	//传递给任务函数
													(UBaseType_t )	task_list[i].priority, 	//任务优先级
													(StackType_t* )	Task_Stack[i], 			//任务栈
													(StaticTask_t*)&Task_TCB[i]); 			//任务控制块
		if (task_list[i].handle != NULL){
#ifdef SYSTEM_UART_DEBUG_CONSOLE
			PRINTF("Create ""%s"" successfully.\tTask Index = %d\n",task_list[i].name,i);
#endif
		}else{
#ifdef SYSTEM_UART_DEBUG_CONSOLE
			PRINTF("Create ""%s"" failed.\tTask Index = %d\n",task_list[i].name,i);	
#endif
		}
	}

	vTaskDelete(AppTaskCreate_Handle);
	taskEXIT_CRITICAL();
}

void OperatingSystem::start(void){

	AppTaskCreate_Handle = xTaskCreateStatic(	(TaskFunction_t )OperatingSystem::initTasks,  /* 任务入口函数 */
                        						(const char*    )"AppTaskCreate",/* 任务名字 */
                        						(uint16_t       )128,  /* 任务栈大小 */
                        						(void*          )NULL,/* 任务入口函数参数 */
                        						(UBaseType_t    )3, /* 任务的优先级 */
                        						(StackType_t*   )AppTaskCreate_Stack,/* 任务控制块指针 */ 
                        						(StaticTask_t*  )&AppTaskCreate_TCB);
	if(AppTaskCreate_Handle != NULL)
    	vTaskStartScheduler();   /* 启动任务，开启调度 */
  	else
    	while(1);
}


// API for FreeRTOS ==============================================================================

StaticTask_t* EXTERN_C_TIMER_TASK_TCB    = &OperatingSystem::Timer_Task_TCB;
StackType_t * EXTERN_C_TIMER_TASK_STACK  =  OperatingSystem::Timer_Task_Stack;
StaticTask_t* EXTERN_C_IDLE_TASK_TCB     = &OperatingSystem::Idle_Task_TCB;
StackType_t * EXTERN_C_IDLE_TASK_STACK   =  OperatingSystem::Idle_Task_Stack;
#ifdef __cplusplus
extern "C"{
#endif

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize){
	*ppxTimerTaskTCBBuffer   = EXTERN_C_TIMER_TASK_TCB;			// 定时器任务控制块内存
	*ppxTimerTaskStackBuffer = EXTERN_C_TIMER_TASK_STACK;		// 定时器任务控制块堆栈
	*pulTimerTaskStackSize   = configTIMER_TASK_STACK_DEPTH;	// 定时器任务堆栈大小
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, \
									StackType_t **ppxIdleTaskStackBuffer, \
									uint32_t *pulIdleTaskStackSize){
	*ppxIdleTaskTCBBuffer	 = EXTERN_C_IDLE_TASK_TCB;			// 空闲任务控制块内存
	*ppxIdleTaskStackBuffer  = EXTERN_C_IDLE_TASK_STACK;		// 空闲任务控制块堆栈
	*pulIdleTaskStackSize	 =  configMINIMAL_STACK_SIZE;		// 空闲任务堆栈大小
}
#ifdef __cplusplus
}
#endif
//================================================================================================

