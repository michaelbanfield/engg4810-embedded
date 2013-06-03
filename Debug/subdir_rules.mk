################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
buttons.obj: ../buttons.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=5 -g --include_path="C:/ti/ccsv5/tools/compiler/tms470_4.9.5/include" --include_path="C:/Users/Michael/Downloads/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" --include_path="C:/StellarisWare3/boards/ek-lm4f232" --include_path="C:/StellarisWare3" --include_path="C:/StellarisWare3/third_party" --fp_mode=relaxed --gcc --define=ccs="ccs" --define=ARM_MATH_CM4 --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1 --diag_warning=225 --display_error_number --gen_func_subsections=on --fp_reassoc=on --ual --preproc_with_compile --preproc_dependency="buttons.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ff.obj: ../ff.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=5 -g --include_path="C:/ti/ccsv5/tools/compiler/tms470_4.9.5/include" --include_path="C:/Users/Michael/Downloads/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" --include_path="C:/StellarisWare3/boards/ek-lm4f232" --include_path="C:/StellarisWare3" --include_path="C:/StellarisWare3/third_party" --fp_mode=relaxed --gcc --define=ccs="ccs" --define=ARM_MATH_CM4 --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1 --diag_warning=225 --display_error_number --gen_func_subsections=on --fp_reassoc=on --ual --preproc_with_compile --preproc_dependency="ff.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

interrupts.obj: ../interrupts.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=5 -g --include_path="C:/ti/ccsv5/tools/compiler/tms470_4.9.5/include" --include_path="C:/Users/Michael/Downloads/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" --include_path="C:/StellarisWare3/boards/ek-lm4f232" --include_path="C:/StellarisWare3" --include_path="C:/StellarisWare3/third_party" --fp_mode=relaxed --gcc --define=ccs="ccs" --define=ARM_MATH_CM4 --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1 --diag_warning=225 --display_error_number --gen_func_subsections=on --fp_reassoc=on --ual --preproc_with_compile --preproc_dependency="interrupts.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

math_helper.obj: ../math_helper.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=5 -g --include_path="C:/ti/ccsv5/tools/compiler/tms470_4.9.5/include" --include_path="C:/Users/Michael/Downloads/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" --include_path="C:/StellarisWare3/boards/ek-lm4f232" --include_path="C:/StellarisWare3" --include_path="C:/StellarisWare3/third_party" --fp_mode=relaxed --gcc --define=ccs="ccs" --define=ARM_MATH_CM4 --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1 --diag_warning=225 --display_error_number --gen_func_subsections=on --fp_reassoc=on --ual --preproc_with_compile --preproc_dependency="math_helper.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=5 -g --include_path="C:/ti/ccsv5/tools/compiler/tms470_4.9.5/include" --include_path="C:/Users/Michael/Downloads/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" --include_path="C:/StellarisWare3/boards/ek-lm4f232" --include_path="C:/StellarisWare3" --include_path="C:/StellarisWare3/third_party" --fp_mode=relaxed --gcc --define=ccs="ccs" --define=ARM_MATH_CM4 --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1 --diag_warning=225 --display_error_number --gen_func_subsections=on --fp_reassoc=on --ual --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_dev_msc.obj: ../usb_dev_msc.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=5 -g --include_path="C:/ti/ccsv5/tools/compiler/tms470_4.9.5/include" --include_path="C:/Users/Michael/Downloads/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" --include_path="C:/StellarisWare3/boards/ek-lm4f232" --include_path="C:/StellarisWare3" --include_path="C:/StellarisWare3/third_party" --fp_mode=relaxed --gcc --define=ccs="ccs" --define=ARM_MATH_CM4 --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1 --diag_warning=225 --display_error_number --gen_func_subsections=on --fp_reassoc=on --ual --preproc_with_compile --preproc_dependency="usb_dev_msc.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usb_msc_structs.obj: ../usb_msc_structs.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=5 -g --include_path="C:/ti/ccsv5/tools/compiler/tms470_4.9.5/include" --include_path="C:/Users/Michael/Downloads/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" --include_path="C:/StellarisWare3/boards/ek-lm4f232" --include_path="C:/StellarisWare3" --include_path="C:/StellarisWare3/third_party" --fp_mode=relaxed --gcc --define=ccs="ccs" --define=ARM_MATH_CM4 --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1 --diag_warning=225 --display_error_number --gen_func_subsections=on --fp_reassoc=on --ual --preproc_with_compile --preproc_dependency="usb_msc_structs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usbdsdcard.obj: ../usbdsdcard.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/tms470_4.9.5/bin/cl470" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=5 -g --include_path="C:/ti/ccsv5/tools/compiler/tms470_4.9.5/include" --include_path="C:/Users/Michael/Downloads/CMSIS-SP-00300-r3p1-00rel0/CMSIS/Include" --include_path="C:/StellarisWare3/boards/ek-lm4f232" --include_path="C:/StellarisWare3" --include_path="C:/StellarisWare3/third_party" --fp_mode=relaxed --gcc --define=ccs="ccs" --define=ARM_MATH_CM4 --define=PART_LM4F232H5QD --define=TARGET_IS_BLIZZARD_RA1 --diag_warning=225 --display_error_number --gen_func_subsections=on --fp_reassoc=on --ual --preproc_with_compile --preproc_dependency="usbdsdcard.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


