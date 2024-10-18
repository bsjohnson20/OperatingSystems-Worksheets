all: task1 task2 
	@echo "compiling all tasks:"
	
	
task1: 
	./compiling.sh 1 0


task2: src/task2.asm
	./compiling.sh 2 0



