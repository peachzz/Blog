/**
  @page STemWin performance application Readme file
 
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    STM324x9I-EVAL/Performance/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-September-2016    
  * @brief   Description of STemWin Performance application.   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
   @endverbatim

@par Application Description

This application is developed to evaluate the drawing performance for STemWin Library.
All the tests are run sequentially and at the end, the result is shown on the screen. 

Please note that you can change the pixel format in the "LCDConf_stm324x9i_eval_MB1046.c" 
file to show the performance with differents bpp configuration (32/24/16/8)

#define COLOR_CONVERSION_0 GUICC_M8888I  /  GUICC_M888    /  GUICC_M565    /  GUICC_8666
#define DISPLAY_DRIVER_0   GUIDRV_LIN_32 /  GUIDRV_LIN_24 /  GUIDRV_LIN_16 /  GUIDRV_LIN_8

@par Directory contents 

  - STM324x9I-EVAL/Performance/Config/GUIConf_stm324x9i_eval.h         Header for GUIConf*.c
  - STM324x9I-EVAL/Performance/Config/LCDConf_stm324x9i_eval.h         Header for LCDConf*.c
  - STM324x9I-EVAL/Performance/User/bsp.h                              Header for bsp.c
  - STM324x9I-EVAL/Performance/User/main.h                             Main program header file
  - STM324x9I-EVAL/Performance/User/stm32f4xx_conf.h                   Library Configuration file
  - STM324x9I-EVAL/Performance/User/stm32f4xx_it.h                     Interrupt handlers header file
  - STM324x9I-EVAL/Performance/Config/LCDConf_stm324x9i_eval_MB1046.c  Display controller initialization file (MB1046)
  - STM324x9I-EVAL/Performance/Config/GUIConf_stm324x9i_eval.c         Configuration file for the GUI library
  - STM324x9I-EVAL/Performance/User/main.c                             Main program file
  - STM324x9I-EVAL/Performance/User/bsp.c                              Targets hardware configuration
  - STM324x9I-EVAL/Performance/User/stm32xxx_it.c                      STM32xxx Interrupt handlers
  - STM324x9I-EVAL/Performance/User/system_stm32f4xx.c                 STM32F4xx system file
  - STM324x9I-EVAL/Performance/User/BASIC_DriverPerformance.c          Display driver performance test

@par Hardware and Software environment  

  - This application runs on STM32F429xx/439xx devices.

  - This application has been tested with STMicroelectronics STM324x9I-EVAL 
    boards and can be easily tailored to any other supported device 
    and development board.
	
  
@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
