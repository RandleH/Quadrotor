/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

*/


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H



//Õë¶Ô²»Í¬µÄ±àÒëÆ÷µ÷ÓÃ²»Í¬µÄstdint.hÎÄ¼þ
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    #include "fsl_debug_console.h"
    extern uint32_t SystemCoreClock;

#endif



//¶ÏÑÔ
#define vAssertCalled(char,int) PRINTF("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)

/************************************************************************
 *               FreeRTOS»ù´¡ÅäÖÃÅäÖÃÑ¡Ïî 
 *********************************************************************/
/* ÖÃ1£ºRTOSÊ¹ÓÃÇÀÕ¼Ê½µ÷¶ÈÆ÷£»ÖÃ0£ºRTOSÊ¹ÓÃÐ­×÷Ê½µ÷¶ÈÆ÷£¨Ê±¼äÆ¬£©
 * 
 * ×¢£ºÔÚ¶àÈÎÎñ¹ÜÀí»úÖÆÉÏ£¬²Ù×÷ÏµÍ³¿ÉÒÔ·ÖÎªÇÀÕ¼Ê½ºÍÐ­×÷Ê½Á½ÖÖ¡£
 * Ð­×÷Ê½²Ù×÷ÏµÍ³ÊÇÈÎÎñÖ÷¶¯ÊÍ·ÅCPUºó£¬ÇÐ»»µ½ÏÂÒ»¸öÈÎÎñ¡£
 * ÈÎÎñÇÐ»»µÄÊ±»úÍêÈ«È¡¾öÓÚÕýÔÚÔËÐÐµÄÈÎÎñ¡£
 */
#define configUSE_PREEMPTION					  1

//1Ê¹ÄÜÊ±¼äÆ¬µ÷¶È(Ä¬ÈÏÊ½Ê¹ÄÜµÄ)
#define configUSE_TIME_SLICING					1		

/* Ä³Ð©ÔËÐÐFreeRTOSµÄÓ²¼þÓÐÁ½ÖÖ·½·¨Ñ¡ÔñÏÂÒ»¸öÒªÖ´ÐÐµÄÈÎÎñ£º
 * Í¨ÓÃ·½·¨ºÍÌØ¶¨ÓÚÓ²¼þµÄ·½·¨£¨ÒÔÏÂ¼ò³Æ¡°ÌØÊâ·½·¨¡±£©¡£
 * 
 * Í¨ÓÃ·½·¨£º
 *      1.configUSE_PORT_OPTIMISED_TASK_SELECTION Îª 0 »òÕßÓ²¼þ²»Ö§³ÖÕâÖÖÌØÊâ·½·¨¡£
 *      2.¿ÉÒÔÓÃÓÚËùÓÐFreeRTOSÖ§³ÖµÄÓ²¼þ
 *      3.ÍêÈ«ÓÃCÊµÏÖ£¬Ð§ÂÊÂÔµÍÓÚÌØÊâ·½·¨¡£
 *      4.²»Ç¿ÖÆÒªÇóÏÞÖÆ×î´ó¿ÉÓÃÓÅÏÈ¼¶ÊýÄ¿
 * ÌØÊâ·½·¨£º
 *      1.±ØÐë½«configUSE_PORT_OPTIMISED_TASK_SELECTIONÉèÖÃÎª1¡£
 *      2.ÒÀÀµÒ»¸ö»ò¶à¸öÌØ¶¨¼Ü¹¹µÄ»ã±àÖ¸Áî£¨Ò»°ãÊÇÀàËÆ¼ÆËãÇ°µ¼Áã[CLZ]Ö¸Áî£©¡£
 *      3.±ÈÍ¨ÓÃ·½·¨¸ü¸ßÐ§
 *      4.Ò»°ãÇ¿ÖÆÏÞ¶¨×î´ó¿ÉÓÃÓÅÏÈ¼¶ÊýÄ¿Îª32
 * Ò»°ãÊÇÓ²¼þ¼ÆËãÇ°µ¼ÁãÖ¸Áî£¬Èç¹ûËùÊ¹ÓÃµÄ£¬MCUÃ»ÓÐÕâÐ©Ó²¼þÖ¸ÁîµÄ»°´ËºêÓ¦¸ÃÉèÖÃÎª0£¡
 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	        1                       
                                                                        
/* ÖÃ1£ºÊ¹ÄÜµÍ¹¦ºÄticklessÄ£Ê½£»ÖÃ0£º±£³ÖÏµÍ³½ÚÅÄ£¨tick£©ÖÐ¶ÏÒ»Ö±ÔËÐÐ
 * ¼ÙÉè¿ªÆôµÍ¹¦ºÄµÄ»°¿ÉÄÜ»áµ¼ÖÂÏÂÔØ³öÏÖÎÊÌâ£¬ÒòÎª³ÌÐòÔÚË¯ÃßÖÐ,¿ÉÓÃÒÔÏÂ°ì·¨½â¾ö
 * 
 * ÏÂÔØ·½·¨£º
 *      1.½«¿ª·¢°æÕý³£Á¬½ÓºÃ
 *      2.°´×¡¸´Î»°´¼ü£¬µã»÷ÏÂÔØË²¼äËÉ¿ª¸´Î»°´¼ü
 *     
 *      1.Í¨¹ýÌøÏßÃ±½« BOOT 0 ½Ó¸ßµçÆ½(3.3V)
 *      2.ÖØÐÂÉÏµç£¬ÏÂÔØ
 *    
 * 			1.Ê¹ÓÃFlyMcu²Á³ýÒ»ÏÂÐ¾Æ¬£¬È»ºó½øÐÐÏÂÔØ
 *			STMISP -> Çå³ýÐ¾Æ¬(z)
 */
#define configUSE_TICKLESS_IDLE													0   

/*
 * Ð´ÈëÊµ¼ÊµÄCPUÄÚºËÊ±ÖÓÆµÂÊ£¬Ò²¾ÍÊÇCPUÖ¸ÁîÖ´ÐÐÆµÂÊ£¬Í¨³£³ÆÎªFclk
 * FclkÎª¹©¸øCPUÄÚºËµÄÊ±ÖÓÐÅºÅ£¬ÎÒÃÇËùËµµÄcpuÖ÷ÆµÎª XX MHz£¬
 * ¾ÍÊÇÖ¸µÄÕâ¸öÊ±ÖÓÐÅºÅ£¬ÏàÓ¦µÄ£¬1/Fclk¼´ÎªcpuÊ±ÖÓÖÜÆÚ£»
 */
#define configCPU_CLOCK_HZ						  (SystemCoreClock)

//RTOSÏµÍ³½ÚÅÄÖÐ¶ÏµÄÆµÂÊ¡£¼´Ò»ÃëÖÐ¶ÏµÄ´ÎÊý£¬Ã¿´ÎÖÐ¶ÏRTOS¶¼»á½øÐÐÈÎÎñµ÷¶È
#define configTICK_RATE_HZ						  (( TickType_t )1000)

//¿ÉÊ¹ÓÃµÄ×î´óÓÅÏÈ¼¶
#define configMAX_PRIORITIES					  (32)

//¿ÕÏÐÈÎÎñÊ¹ÓÃµÄ¶ÑÕ»´óÐ¡
#define configMINIMAL_STACK_SIZE				((unsigned short)128)
  
//ÈÎÎñÃû×Ö×Ö·û´®³¤¶È
#define configMAX_TASK_NAME_LEN					(16)

 //ÏµÍ³½ÚÅÄ¼ÆÊýÆ÷±äÁ¿Êý¾ÝÀàÐÍ£¬1±íÊ¾Îª16Î»ÎÞ·ûºÅÕûÐÎ£¬0±íÊ¾Îª32Î»ÎÞ·ûºÅÕûÐÎ
#define configUSE_16_BIT_TICKS					0                      

//¿ÕÏÐÈÎÎñ·ÅÆúCPUÊ¹ÓÃÈ¨¸øÆäËûÍ¬ÓÅÏÈ¼¶µÄÓÃ»§ÈÎÎñ
#define configIDLE_SHOULD_YIELD					1           

//ÆôÓÃ¶ÓÁÐ
#define configUSE_QUEUE_SETS					  0    

//¿ªÆôÈÎÎñÍ¨Öª¹¦ÄÜ£¬Ä¬ÈÏ¿ªÆô
#define configUSE_TASK_NOTIFICATIONS    1   

//Ê¹ÓÃ»¥³âÐÅºÅÁ¿
#define configUSE_MUTEXES						    0    

//Ê¹ÓÃµÝ¹é»¥³âÐÅºÅÁ¿                                            
#define configUSE_RECURSIVE_MUTEXES			0   

//Îª1Ê±Ê¹ÓÃ¼ÆÊýÐÅºÅÁ¿
#define configUSE_COUNTING_SEMAPHORES		0

/* ÉèÖÃ¿ÉÒÔ×¢²áµÄÐÅºÅÁ¿ºÍÏûÏ¢¶ÓÁÐ¸öÊý */
#define configQUEUE_REGISTRY_SIZE				10                                 
                                                                       
#define configUSE_APPLICATION_TASK_TAG		  0                       
                      

/*****************************************************************
              FreeRTOSÓëÄÚ´æÉêÇëÓÐ¹ØÅäÖÃÑ¡Ïî                                               
*****************************************************************/
//Ö§³Ö¶¯Ì¬ÄÚ´æÉêÇë
#define configSUPPORT_DYNAMIC_ALLOCATION        1    
//Ö§³Ö¾²Ì¬ÄÚ´æ
#define configSUPPORT_STATIC_ALLOCATION					1					
//ÏµÍ³ËùÓÐ×ÜµÄ¶Ñ´óÐ¡
#define configTOTAL_HEAP_SIZE					((size_t)(36*1024))    


/***************************************************************
             FreeRTOSÓë¹³×Óº¯ÊýÓÐ¹ØµÄÅäÖÃÑ¡Ïî                                            
**************************************************************/
/* ÖÃ1£ºÊ¹ÓÃ¿ÕÏÐ¹³×Ó£¨Idle HookÀàËÆÓÚ»Øµ÷º¯Êý£©£»ÖÃ0£ººöÂÔ¿ÕÏÐ¹³×Ó
 * 
 * ¿ÕÏÐÈÎÎñ¹³×ÓÊÇÒ»¸öº¯Êý£¬Õâ¸öº¯ÊýÓÉÓÃ»§À´ÊµÏÖ£¬
 * FreeRTOS¹æ¶¨ÁËº¯ÊýµÄÃû×ÖºÍ²ÎÊý£ºvoid vApplicationIdleHook(void )£¬
 * Õâ¸öº¯ÊýÔÚÃ¿¸ö¿ÕÏÐÈÎÎñÖÜÆÚ¶¼»á±»µ÷ÓÃ
 * ¶ÔÓÚÒÑ¾­É¾³ýµÄRTOSÈÎÎñ£¬¿ÕÏÐÈÎÎñ¿ÉÒÔÊÍ·Å·ÖÅä¸øËüÃÇµÄ¶ÑÕ»ÄÚ´æ¡£
 * Òò´Ë±ØÐë±£Ö¤¿ÕÏÐÈÎÎñ¿ÉÒÔ±»CPUÖ´ÐÐ
 * Ê¹ÓÃ¿ÕÏÐ¹³×Óº¯ÊýÉèÖÃCPU½øÈëÊ¡µçÄ£Ê½ÊÇºÜ³£¼ûµÄ
 * ²»¿ÉÒÔµ÷ÓÃ»áÒýÆð¿ÕÏÐÈÎÎñ×èÈûµÄAPIº¯Êý
 */
#define configUSE_IDLE_HOOK						0      

/* ÖÃ1£ºÊ¹ÓÃÊ±¼äÆ¬¹³×Ó£¨Tick Hook£©£»ÖÃ0£ººöÂÔÊ±¼äÆ¬¹³×Ó
 * 
 * 
 * Ê±¼äÆ¬¹³×ÓÊÇÒ»¸öº¯Êý£¬Õâ¸öº¯ÊýÓÉÓÃ»§À´ÊµÏÖ£¬
 * FreeRTOS¹æ¶¨ÁËº¯ÊýµÄÃû×ÖºÍ²ÎÊý£ºvoid vApplicationTickHook(void )
 * Ê±¼äÆ¬ÖÐ¶Ï¿ÉÒÔÖÜÆÚÐÔµÄµ÷ÓÃ
 * º¯Êý±ØÐë·Ç³£¶ÌÐ¡£¬²»ÄÜ´óÁ¿Ê¹ÓÃ¶ÑÕ»£¬
 * ²»ÄÜµ÷ÓÃÒÔ¡±FromISR" »ò "FROM_ISR¡±½áÎ²µÄAPIº¯Êý
 */
 /*xTaskIncrementTickº¯ÊýÊÇÔÚxPortSysTickHandlerÖÐ¶Ïº¯ÊýÖÐ±»µ÷ÓÃµÄ¡£Òò´Ë£¬vApplicationTickHook()º¯ÊýÖ´ÐÐµÄÊ±¼ä±ØÐëºÜ¶Ì²ÅÐÐ*/
#define configUSE_TICK_HOOK						0           

//Ê¹ÓÃÄÚ´æÉêÇëÊ§°Ü¹³×Óº¯Êý
#define configUSE_MALLOC_FAILED_HOOK			0 

/*
 * ´óÓÚ0Ê±ÆôÓÃ¶ÑÕ»Òç³ö¼ì²â¹¦ÄÜ£¬Èç¹ûÊ¹ÓÃ´Ë¹¦ÄÜ 
 * ÓÃ»§±ØÐëÌá¹©Ò»¸öÕ»Òç³ö¹³×Óº¯Êý£¬Èç¹ûÊ¹ÓÃµÄ»°
 * ´ËÖµ¿ÉÒÔÎª1»òÕß2£¬ÒòÎªÓÐÁ½ÖÖÕ»Òç³ö¼ì²â·½·¨ */
#define configCHECK_FOR_STACK_OVERFLOW			0   


/********************************************************************
          FreeRTOSÓëÔËÐÐÊ±¼äºÍÈÎÎñ×´Ì¬ÊÕ¼¯ÓÐ¹ØµÄÅäÖÃÑ¡Ïî   
**********************************************************************/
//ÆôÓÃÔËÐÐÊ±¼äÍ³¼Æ¹¦ÄÜ
#define configGENERATE_RUN_TIME_STATS	        0             
 //ÆôÓÃ¿ÉÊÓ»¯¸ú×Ùµ÷ÊÔ
#define configUSE_TRACE_FACILITY				      0    
/* ÓëºêconfigUSE_TRACE_FACILITYÍ¬Ê±Îª1Ê±»á±àÒëÏÂÃæ3¸öº¯Êý
 * prvWriteNameToBuffer()
 * vTaskList(),
 * vTaskGetRunTimeStats()
*/
#define configUSE_STATS_FORMATTING_FUNCTIONS	1                       
                                                                        
                                                                        
/********************************************************************
                FreeRTOSÓëÐ­³ÌÓÐ¹ØµÄÅäÖÃÑ¡Ïî                                                
*********************************************************************/
//ÆôÓÃÐ­³Ì£¬ÆôÓÃÐ­³ÌÒÔºó±ØÐëÌí¼ÓÎÄ¼þcroutine.c
#define configUSE_CO_ROUTINES 			          0                 
//Ð­³ÌµÄÓÐÐ§ÓÅÏÈ¼¶ÊýÄ¿
#define configMAX_CO_ROUTINE_PRIORITIES       ( 2 )                   


/***********************************************************************
                FreeRTOSÓëÈí¼þ¶¨Ê±Æ÷ÓÐ¹ØµÄÅäÖÃÑ¡Ïî      
**********************************************************************/
 //ÆôÓÃÈí¼þ¶¨Ê±Æ÷
#define configUSE_TIMERS				            0                              
//Èí¼þ¶¨Ê±Æ÷ÓÅÏÈ¼¶
#define configTIMER_TASK_PRIORITY		        (configMAX_PRIORITIES-1)        
//Èí¼þ¶¨Ê±Æ÷¶ÓÁÐ³¤¶È
#define configTIMER_QUEUE_LENGTH					10                               
//Èí¼þ¶¨Ê±Æ÷ÈÎÎñ¶ÑÕ»´óÐ¡
#define configTIMER_TASK_STACK_DEPTH	      (configMINIMAL_STACK_SIZE*2)    

/************************************************************
            FreeRTOS¿ÉÑ¡º¯ÊýÅäÖÃÑ¡Ïî                                                     
************************************************************/
#define INCLUDE_xTaskGetSchedulerState				1                       
#define INCLUDE_vTaskPrioritySet					1
#define INCLUDE_uxTaskPriorityGet					1
#define INCLUDE_vTaskDelete							1
#define INCLUDE_vTaskCleanUpResources				1
#define INCLUDE_vTaskSuspend						1
#define INCLUDE_vTaskDelayUntil						1
#define INCLUDE_vTaskDelay  						1
#define INCLUDE_eTaskGetState						1
#define INCLUDE_xTimerPendFunctionCall	     		0
//#define INCLUDE_xTaskGetCurrentTaskHandle       1
//#define INCLUDE_uxTaskGetStackHighWaterMark     0
//#define INCLUDE_xTaskGetIdleTaskHandle          0


/******************************************************************
            FreeRTOSÓëÖÐ¶ÏÓÐ¹ØµÄÅäÖÃÑ¡Ïî                                                 
******************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4                  
#endif
//ÖÐ¶Ï×îµÍÓÅÏÈ¼¶
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15     

//ÏµÍ³¿É¹ÜÀíµÄ×î¸ßÖÐ¶ÏÓÅÏÈ¼¶
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5 

#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )	/* 240 */

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )


/****************************************************************
            FreeRTOSÓëÖÐ¶Ï·þÎñº¯ÊýÓÐ¹ØµÄÅäÖÃÑ¡Ïî                         
****************************************************************/
#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler


/* ÒÔÏÂÎªÊ¹ÓÃPercepio TracealyzerÐèÒªµÄ¶«Î÷£¬²»ÐèÒªÊ±½« configUSE_TRACE_FACILITY ¶¨ÒåÎª 0 */
#if ( configUSE_TRACE_FACILITY == 1 )
#include "trcRecorder.h"
#define INCLUDE_xTaskGetCurrentTaskHandle               1   // ÆôÓÃÒ»¸ö¿ÉÑ¡º¯Êý£¨¸Ãº¯Êý±» TraceÔ´ÂëÊ¹ÓÃ£¬Ä¬ÈÏ¸ÃÖµÎª0 ±íÊ¾²»ÓÃ£©
#endif


#endif /* FREERTOS_CONFIG_H */

