################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/ComparatorA.obj: ../src/ComparatorA.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.4/bin/cl430" -vmsp --abi=coffabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/rainf_000/Desktop/G2 pocket lab code for CCS5.1/17_Slope_ADC/src" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.4/include" --include_path="C:/Users/rainf_000/Desktop/G2 pocket lab code for CCS5.1/17_Slope_ADC/src/TCA6416A" --define=__MSP430G2553__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="src/ComparatorA.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Slope.obj: ../src/Slope.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.4/bin/cl430" -vmsp --abi=coffabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/rainf_000/Desktop/G2 pocket lab code for CCS5.1/17_Slope_ADC/src" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.4/include" --include_path="C:/Users/rainf_000/Desktop/G2 pocket lab code for CCS5.1/17_Slope_ADC/src/TCA6416A" --define=__MSP430G2553__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="src/Slope.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Timer_A3.obj: ../src/Timer_A3.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.4/bin/cl430" -vmsp --abi=coffabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/rainf_000/Desktop/G2 pocket lab code for CCS5.1/17_Slope_ADC/src" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.4/include" --include_path="C:/Users/rainf_000/Desktop/G2 pocket lab code for CCS5.1/17_Slope_ADC/src/TCA6416A" --define=__MSP430G2553__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="src/Timer_A3.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


