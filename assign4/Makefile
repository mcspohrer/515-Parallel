#----------------------------------------------------------------------
# Makefile for CS415/515 Assignment 4
#----------------------------------------------------------------------

sampleSort: sampleSort.c
	mpicc -o sampleSort sampleSort.c -g -lm 

%: %.c
	gcc -o $@ $< -g -lm
