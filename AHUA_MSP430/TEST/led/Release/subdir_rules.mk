################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/MCDEVELOPER/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmsp -O2 --use_hw_mpy=none --include_path="D:/MCDEVELOPER/ccs/ccs_base/msp430/include" --include_path="E:/.A/A6_GITS/Teyvant_MEOS/AHUA_MSP430/TEST/led" --include_path="D:/MCDEVELOPER/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power=all --define=__MSP430G2533__ --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


