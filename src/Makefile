default: main.cpp OMPScorer.cpp SeqStateScorer.cpp PthreadStateScorer.cpp board.h CudaScorer.cpp Kernel.cu
	nvcc -std=c++11 -Xcompiler -fopenmp Kernel.cu SeqStateScorer.cpp PthreadStateScorer.cpp OMPScorer.cpp CudaScorer.cpp main.cpp

clean:
	rm *.out
