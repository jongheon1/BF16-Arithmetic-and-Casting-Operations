####################################################################################################
#    CSI2107 Project 1
#---------------------------------------------------------------------------------------------------
#    .
#    ├── Makefile
#    ├── examples        // example input and output files
#    │   ├── input1.txt
#    │   └── input2.txt
#    │   ├── input1.out
#    │   └── input2.out
#    ├── bf16.h
#    ├── bf16.c      // fill this code
#    └── main.c      // the main function
#
#    === How to run ==
#    $ make
#    $ ./main < examples/input1.txt
#
#    === How to submit your source code ==
#    $ make tar
#    Then, upload your tar file to LearnUs.
#
###################################################################################################-

main: ../../../../../../CLionProjects/untitled/main.c ../../../../../../CLionProjects/untitled/bf16.c
	gcc -o main main.c bf16.c

clean:
	rm -f main
	rm -f ${USER}.tar

tar:
	tar -cf ${USER}.tar bf16.c
	@echo "================================================================================="
	@echo "${USER}.tar is generated. This does NOT mean your solution is submitted"
	@echo "You MUST upload ${USER}.tar to LearnUs."
	@echo "You may need to use scp to copy ${USER}.tar from this server to your computer"
	@echo "If you have any difficulty, please contact TA (not your colleague)"

