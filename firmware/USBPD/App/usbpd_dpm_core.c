/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_dpm_core.c
  * @author  MCD Application Team
  * @brief   USBPD dpm core file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#define __USBPD_DPM_CORE_C

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_trace.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_user.h"
#include "usbpd_dpm_conf.h"

#if defined(_LOW_POWER)
#include "usbpd_lowpower.h"
#endif /* _LOW_POWER */

/* OS management */
#include "usbpd_os_port.h"

/* Private definition -------------------------------------------------------*/
/* function import prototypes -----------------------------------------------*/
/* Generic STM32 prototypes */
extern uint32_t HAL_GetTick(void);

/* Private function prototypes -----------------------------------------------*/
void USBPD_CAD_Task(void);
void USBPD_TaskUser(void);

#if defined(USE_STM32_UTILITY_OS)
void TimerCADfunction(void *);
#endif /* USE_STM32_UTILITY_OS */

#if defined(USE_STM32_UTILITY_OS)
void USBPD_PE_Task_P0(void);
void USBPD_PE_Task_P1(void);
void TimerPE0function(void *pArg);
void TimerPE1function(void *pArg);
#endif /* USE_STM32_UTILITY_OS */

/* Private typedef -----------------------------------------------------------*/
#if defined(USE_STM32_UTILITY_OS)
UTIL_TIMER_Object_t TimerCAD;
UTIL_TIMER_Object_t TimerPE0, TimerPE1;
#endif /* USE_STM32_UTILITY_OS */

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CHECK_PE_FUNCTION_CALL(_function_)  do{                                     \
                                                _retr = _function_;                  \
                                               if(USBPD_OK != _retr) {goto error;}   \
                                              } while(0);

#define CHECK_CAD_FUNCTION_CALL(_function_) if(USBPD_CAD_OK != _function_)      \
  {                                   \
    _retr = USBPD_ERROR;              \
    goto error;                       \
  }

#if defined(_DEBUG_TRACE)
#define DPM_CORE_DEBUG_TRACE(_PORTNUM_, __MESSAGE__)  \
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, _PORTNUM_, 0u, (uint8_t *)(__MESSAGE__), sizeof(__MESSAGE__) - 1u);
#else
#define DPM_CORE_DEBUG_TRACE(_PORTNUM_, __MESSAGE__)
#endif /* _DEBUG_TRACE */

/* Private variables ---------------------------------------------------------*/
#if !defined(USE_STM32_UTILITY_OS)
#define OFFSET_CAD 1U
static uint32_t DPM_Sleep_time[USBPD_PORT_COUNT + OFFSET_CAD];
static uint32_t DPM_Sleep_start[USBPD_PORT_COUNT + OFFSET_CAD];
#endif /* !USE_STM32_UTILITY_OS */

USBPD_ParamsTypeDef   DPM_Params[USBPD_PORT_COUNT];
/* Private function prototypes -----------------------------------------------*/
static void USBPD_PE_TaskWakeUp(uint8_t PortNum);
static void DPM_StartPETask(uint8_t PortNum);

void USBPD_DPM_CADCallback(uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef Cc);

static void USBPD_DPM_CADTaskWakeUp(void);

/**
  * @brief  Initialize the core stack (port power role, PWR_IF, CAD and PE Init procedures)
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_DPM_InitCore(void)
{
  /* variable to get dynamique memory allocated by usbpd stack */
  uint32_t stack_dynamemsize;
  USBPD_StatusTypeDef _retr = USBPD_OK;

  /* CAD callback definition */
  static const USBPD_PE_Callbacks dpmCallbacks =
  {
    USBPD_DPM_SetupNewPower,
    USBPD_DPM_HardReset,
    NULL,
    USBPD_DPM_Notification,
    USBPD_DPM_ExtendedMessageReceived,
    USBPD_DPM_GetDataInfo,
    USBPD_DPM_SetDataInfo,
    USBPD_DPM_EvaluateRequest,
    NULL,
    NULL,
    USBPD_PE_TaskWakeUp,
#if defined(_VCONN_SUPPORT)
    USBPD_DPM_EvaluateVconnSwap,
    USBPD_DPM_PE_VconnPwr,
#else
    NULL,
    NULL,
#endif /* _VCONN_SUPPORT */
    USBPD_DPM_EnterErrorRecovery,
    USBPD_DPM_EvaluateDataRoleSwap,
    USBPD_DPM_IsPowerReady
  };

  static const USBPD_CAD_Callbacks CAD_cbs =
  {
    USBPD_DPM_CADCallback,
    USBPD_DPM_CADTaskWakeUp
  };

  /* Check the lib selected */
  if (USBPD_TRUE != USBPD_PE_CheckLIB(LIB_ID))
  {
    _retr = USBPD_ERROR;
    goto error;
  }

  /* to get how much memory are dynamically allocated by the stack
     the memory return is corresponding to 2 ports so if the application
     managed only one port divide the value return by 2                   */
  stack_dynamemsize = USBPD_PE_GetMemoryConsumption();

  /* done to avoid warning */
  (void)stack_dynamemsize;

  for (uint8_t _port_index = 0; _port_index < USBPD_PORT_COUNT; ++_port_index)
  {
    /* Variable to be sure that DPM is correctly initialized */
    DPM_Params[_port_index].DPM_Initialized = USBPD_FALSE;

    /* check the stack settings */
    DPM_Params[_port_index].PE_SpecRevision  = DPM_Settings[_port_index].PE_SpecRevision;
    DPM_Params[_port_index].PE_PowerRole     = DPM_Settings[_port_index].PE_DefaultRole;
    DPM_Params[_port_index].PE_SwapOngoing   = USBPD_FALSE;
    DPM_Params[_port_index].ActiveCCIs       = CCNONE;
    DPM_Params[_port_index].VconnCCIs        = CCNONE;
    DPM_Params[_port_index].VconnStatus      = USBPD_FALSE;

    /* CAD SET UP : Port 0 */
    CHECK_CAD_FUNCTION_CALL(USBPD_CAD_Init(_port_index,
                                           &CAD_cbs,
                                           &DPM_Settings[_port_index],
                                           &DPM_Params[_port_index]));

    /* PE SET UP : Port 0 */
    CHECK_PE_FUNCTION_CALL(USBPD_PE_Init(_port_index, (USBPD_SettingsTypeDef *)&DPM_Settings[_port_index],
                                         &DPM_Params[_port_index], &dpmCallbacks));

    /* DPM is correctly initialized */
    DPM_Params[_port_index].DPM_Initialized = USBPD_TRUE;

    /* Enable CAD on Port 0 */
    USBPD_CAD_PortEnable(_port_index, USBPD_CAD_ENABLE);
  }

#if defined(USE_STM32_UTILITY_OS)
  /* initialise timer server */
  UTIL_TIMER_Init();

  /* initialize the sequencer */
  UTIL_SEQ_Init();
#endif /* USE_STM32_UTILITY_OS */

#ifdef _LOW_POWER
  USBPD_LOWPOWER_Init();
#endif /* _LOW_POWER */

error :
  return _retr;
}

/**
  * @brief  Initialize the OS parts (task, queue,... )
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_DPM_InitOS(void)
{
  USBPD_StatusTypeDef _retr = USBPD_OK;

  return _retr;
}

/**
  * @brief  Initialize the OS parts (port power role, PWR_IF, CAD and PE Init procedures)
  * @retval None
  */
 /* NRTOS */
#if defined(USE_STM32_UTILITY_OS)
/**
  * @brief  Task for CAD processing
  * @retval None
  */
void USBPD_CAD_Task(void)
{
  UTIL_TIMER_Stop(&TimerCAD);
  uint32_t _timing = USBPD_CAD_Process();
  UTIL_TIMER_SetPeriod(&TimerCAD, _timing);
  UTIL_TIMER_Start(&TimerCAD);
}

/**
  * @brief  timer function to wakeup CAD Task
  * @param pArg Pointer on an argument
  * @retval None
  */
void TimerCADfunction(void *pArg)
{
  UTIL_SEQ_SetTask(TASK_CAD, 0);
}

/**
  * @brief  timer function to wakeup PE_0 Task
  * @param pArg Pointer on an argument
  * @retval None
  */
void TimerPE0function(void *pArg)
{
  UTIL_SEQ_SetTask(TASK_PE_0, 0);
}

/**
  * @brief  timer function to wakeup PE_1 Task
  * @param pArg Pointer on an argument
  * @retval None
  */
void TimerPE1function(void *pArg)
{
  UTIL_SEQ_SetTask(TASK_PE_1, 0);
}

/**
  * @brief  Task for PE_0 processing
  * @retval None
  */
void USBPD_PE_Task_P0(void)
{
  UTIL_TIMER_Stop(&TimerPE0);
  uint32_t _timing =
    USBPD_PE_StateMachine_SRC(USBPD_PORT_0);
  if (_timing != 0xFFFFFFFF)
  {
    UTIL_TIMER_SetPeriod(&TimerPE0, _timing);
    UTIL_TIMER_Start(&TimerPE0);
  }
}

/**
  * @brief  Task for PE_1 processing
  * @retval None
  */
void USBPD_PE_Task_P1(void)
{
  UTIL_TIMER_Stop(&TimerPE1);
  uint32_t _timing =
    USBPD_PE_StateMachine_SRC(USBPD_PORT_1);
  if (_timing != 0xFFFFFFFF)
  {
    UTIL_TIMER_SetPeriod(&TimerPE1, _timing);
    UTIL_TIMER_Start(&TimerPE1);
  }
}
 /* !USBPDCORE_LIB_NO_PD */

/**
  * @brief  Task for DPM_USER processing
  * @retval None
  */
void USBPD_TaskUser(void)
{
  USBPD_DPM_UserExecute(NULL);
}
#endif /* USE_STM32_UTILITY_OS */

void USBPD_DPM_Run(void)
{
#if defined(USE_STM32_UTILITY_OS)
  UTIL_SEQ_RegTask(TASK_CAD,  0, USBPD_CAD_Task);
  UTIL_SEQ_SetTask(TASK_CAD,  0);
  UTIL_TIMER_Create(&TimerCAD, 10, UTIL_TIMER_ONESHOT, TimerCADfunction, NULL);

  UTIL_SEQ_RegTask(TASK_PE_0, 0,  USBPD_PE_Task_P0);
  UTIL_SEQ_PauseTask(TASK_PE_0);
  UTIL_TIMER_Create(&TimerPE0, 10, UTIL_TIMER_ONESHOT, TimerPE0function, NULL);
#if USBPD_PORT_COUNT == 2
  UTIL_SEQ_RegTask(TASK_PE_1, 0,  USBPD_PE_Task_P1);
  UTIL_SEQ_PauseTask(TASK_PE_1);
  UTIL_TIMER_Create(&TimerPE1, 10, UTIL_TIMER_ONESHOT, TimerPE1function, NULL);
#endif /* USBPD_PORT_COUNT == 2 */

  UTIL_SEQ_RegTask(TASK_USER, 0, USBPD_TaskUser);
  UTIL_SEQ_SetTask(TASK_USER,  0);

  do
  {
    UTIL_SEQ_Run(~0);
  } while (1u == 1u);
#else /* !USE_STM32_UTILITY_OS */
  do
  {
    if ((HAL_GetTick() - DPM_Sleep_start[USBPD_PORT_COUNT]) >= DPM_Sleep_time[USBPD_PORT_COUNT])
    {
      DPM_Sleep_time[USBPD_PORT_COUNT] = USBPD_CAD_Process();
      DPM_Sleep_start[USBPD_PORT_COUNT] = HAL_GetTick();
    }

    uint32_t port = 0;

    for (port = 0; port < USBPD_PORT_COUNT; port++)
    {
      if ((HAL_GetTick() - DPM_Sleep_start[port]) >= DPM_Sleep_time[port])
      {
        DPM_Sleep_time[port] =
          USBPD_PE_StateMachine_SRC(port);
        DPM_Sleep_start[port] = HAL_GetTick();
      }
    }

    USBPD_DPM_UserExecute(NULL);

  } while (1u == 1u);
#endif /* USE_STM32_UTILITY_OS */
}

/**
  * @brief  Initialize DPM (port power role, PWR_IF, CAD and PE Init procedures)
  * @retval USBPD status
  */
void USBPD_DPM_TimerCounter(void)
{
  /* Call PE/PRL timers functions only if DPM is initialized */
  if (USBPD_TRUE == DPM_Params[USBPD_PORT_0].DPM_Initialized)
  {
    USBPD_DPM_UserTimerCounter(USBPD_PORT_0);
    USBPD_PE_TimerCounter(USBPD_PORT_0);
    USBPD_PRL_TimerCounter(USBPD_PORT_0);
  }
#if USBPD_PORT_COUNT==2
  if (USBPD_TRUE == DPM_Params[USBPD_PORT_1].DPM_Initialized)
  {
    USBPD_DPM_UserTimerCounter(USBPD_PORT_1);
    USBPD_PE_TimerCounter(USBPD_PORT_1);
    USBPD_PRL_TimerCounter(USBPD_PORT_1);
  }
#endif /* USBPD_PORT_COUNT == 2 */

}

/**
  * @brief  WakeUp PE task
  * @param  PortNum port number
  * @retval None
  */
static void USBPD_PE_TaskWakeUp(uint8_t PortNum)
{
#if defined(USE_STM32_UTILITY_OS)
  UTIL_SEQ_SetTask(PortNum == 0 ? TASK_PE_0 : TASK_PE_1, 0);
#else
  DPM_Sleep_time[PortNum] = 0;
#endif /* USE_STM32_UTILITY_OS */
}

/**
  * @brief  WakeUp CAD task
  * @retval None
  */
static void USBPD_DPM_CADTaskWakeUp(void)
{
#if defined(USE_STM32_UTILITY_OS)
  UTIL_SEQ_SetTask(TASK_CAD, 0);
#else
  DPM_Sleep_time[USBPD_PORT_COUNT] = 0;
#endif /* USE_STM32_UTILITY_OS */
}

/**
  * @brief  CallBack reporting events on a specified port from CAD layer.
  * @param  PortNum   The handle of the port
  * @param  State     CAD state
  * @param  Cc        The Communication Channel for the USBPD communication
  * @retval None
  */
void USBPD_DPM_CADCallback(uint8_t PortNum, USBPD_CAD_EVENT State, CCxPin_TypeDef Cc)
{
 /* _TRACE */
  (void)(Cc);
  switch (State)
  {

    case USBPD_CAD_EVENT_ATTEMC :
    {
#if defined(_VCONN_SUPPORT)
      DPM_Params[PortNum].VconnStatus = USBPD_TRUE;
#endif /* _VCONN_SUPPORT */
      USBPD_DPM_UserCableDetection(PortNum, USBPD_CAD_EVENT_ATTEMC);
      DPM_StartPETask(PortNum);
      break;
    }
    case USBPD_CAD_EVENT_ATTACHED :
      USBPD_DPM_UserCableDetection(PortNum, USBPD_CAD_EVENT_ATTACHED);
      DPM_StartPETask(PortNum);
      break;

    case USBPD_CAD_EVENT_DETACHED :
    case USBPD_CAD_EVENT_EMC :
    {
      /* Terminate PE task */
#if defined(USE_STM32_UTILITY_OS)
      UTIL_SEQ_PauseTask(PortNum == 0 ? TASK_PE_0 : TASK_PE_1);
#else
      DPM_Sleep_time[PortNum] = 0xFFFFFFFFU;
#endif /* USE_STM32_UTILITY_OS */
      DPM_Params[PortNum].PE_SwapOngoing = USBPD_FALSE;
      DPM_Params[PortNum].PE_Power   = USBPD_POWER_NO;
      USBPD_DPM_UserCableDetection(PortNum, State);
#ifdef _VCONN_SUPPORT
      DPM_Params[PortNum].VconnStatus = USBPD_FALSE;
      DPM_CORE_DEBUG_TRACE(PortNum, "Note: VconnStatus=FALSE");
#endif /* _VCONN_SUPPORT */
      USBPD_DPM_Notification(PortNum, USBPD_NOTIFY_USBSTACK_STOP);
      break;
    }
    default :
      /* nothing to do */
      break;
  }
}

static void DPM_StartPETask(uint8_t PortNum)
{
  USBPD_PE_StateMachine_Reset(PortNum);
#if defined(USE_STM32_UTILITY_OS)
  /* Resume the task */
  UTIL_SEQ_ResumeTask(PortNum == 0 ? TASK_PE_0 : TASK_PE_1);
  /* Enable task execution */
  UTIL_SEQ_SetTask(PortNum == 0 ? TASK_PE_0 : TASK_PE_1, 0);
#else
  DPM_Sleep_time[PortNum] = 0U;
#endif /* USE_STM32_UTILITY_OS */
 /* _RTOS || THREADX */
  USBPD_DPM_Notification(PortNum, USBPD_NOTIFY_USBSTACK_START);
}
 /* USBPDCORE_LIB_NO_PD */

__WEAK void USBPD_DPM_ErrorHandler(void)
{
  /* This function is called to block application execution
     in case of an unexpected behavior
     another solution could be to reset application */
  while (1u == 1u) {};
}
