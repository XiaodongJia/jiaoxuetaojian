################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
DELAY/delay.obj: ../DELAY/delay.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=eabi -Ooff -g --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/USB_API" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/KEY" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/USB_config" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/QRSMAIN" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/SPO2H" --include_path="D:/ti/ccsv6/ccs_base/msp430/include" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/TFT_SCREEN" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/ACCEL" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/ADC12" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/BLOODPREASURE" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/BLUETOOTH" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/driverlib/MSP430F5xx_6xx" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/DELAY" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/UART" --include_path="D:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --include_path="D:/ti/workspace_v6_0/Copy of TI20150113/USB_config" --advice:power="all" --define=__MSP430F5529__ --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="DELAY/delay.pp" --obj_directory="DELAY" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


