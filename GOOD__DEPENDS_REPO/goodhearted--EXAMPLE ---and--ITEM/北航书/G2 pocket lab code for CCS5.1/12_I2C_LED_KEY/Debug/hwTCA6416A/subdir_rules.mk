################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
hwTCA6416A/TCA6416A.obj: ../hwTCA6416A/TCA6416A.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.4/bin/cl430" -vmsp --abi=coffabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.4/include" --include_path="C:/Users/rainf_000/Desktop/G2 pocket lab code for CCS5.1/12_I2C_LED_KEY/hwTCA6416A" --define=__MSP430G2553__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="hwTCA6416A/TCA6416A.pp" --obj_directory="hwTCA6416A" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hwTCA6416A/USCI_I2C.obj: ../hwTCA6416A/USCI_I2C.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.4/bin/cl430" -vmsp --abi=coffabi -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.4/include" --include_path="C:/Users/rainf_000/Desktop/G2 pocket lab code for CCS5.1/12_I2C_LED_KEY/hwTCA6416A" --define=__MSP430G2553__ --diag_warning=225 --display_error_number --printf_support=minimal --preproc_with_compile --preproc_dependency="hwTCA6416A/USCI_I2C.pp" --obj_directory="hwTCA6416A" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


