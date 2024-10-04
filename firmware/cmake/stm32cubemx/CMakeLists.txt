cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_FULL_LL_DRIVER 
	USBPD_PORT_COUNT=2 
	_SRC 
	USBPDCORE_LIB_PD3_FULL 
	USE_HAL_DRIVER 
	STM32G071xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    ../../USBPD/App
    ../../USBPD/Target
    ../../Drivers/STM32G0xx_HAL_Driver/Inc
    ../../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy
    ../../Middlewares/ST/STM32_USBPD_Library/Core/inc
    ../../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/inc
    ../../Drivers/CMSIS/Device/ST/STM32G0xx/Include
    ../../Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c
    ../../Core/Src/gpio.c
    ../../Core/Src/adc.c
    ../../Core/Src/dma.c
    ../../Core/Src/ucpd.c
    ../../Core/Src/stm32g0xx_it.c
    ../../Core/Src/stm32g0xx_hal_msp.c
    ../../USBPD/Target/usbpd_dpm_user.c
    ../../USBPD/Target/usbpd_pwr_user.c
    ../../USBPD/Target/usbpd_vdm_user.c
    ../../USBPD/App/usbpd.c
    ../../USBPD/App/usbpd_pwr_if.c
    ../../USBPD/App/usbpd_dpm_core.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_adc.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_adc_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_adc.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_rcc.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_gpio.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_cortex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_exti.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_utils.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_exti.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim_ex.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_ucpd.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_gpio.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_dma.c
    ../../Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_tim.c
    ../../Core/Src/system_stm32g0xx.c
    ../../Middlewares/ST/STM32_USBPD_Library/Core/src/usbpd_trace.c
    ../../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/src/usbpd_cad_hw_if.c
    ../../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/src/usbpd_hw.c
    ../../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/src/usbpd_hw_if_it.c
    ../../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/src/usbpd_phy.c
    ../../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/src/usbpd_phy_hw_if.c
    ../../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/src/usbpd_pwr_hw_if.c
    ../../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/src/usbpd_timersserver.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../startup_stm32g071xx.s
)

target_link_directories(stm32cubemx INTERFACE
    ../../Middlewares/ST/STM32_USBPD_Library/Core/lib
)

target_link_libraries(stm32cubemx INTERFACE 
	:USBPDCORE_PD3_FULL_CM0PLUS_wc32.a
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


