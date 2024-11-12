OUTPUT_PATH= ./build
OBJECTS= ${OUTPUT_PATH}/WinConsole.o

all:${OBJECTS} 
	g++ ./src/Chip8.cpp ${OBJECTS} -o ${OUTPUT_PATH}/Chip8.exe

${OUTPUT_PATH}/WinConsole.o:src/window/WinConsole.cpp
	g++ ./src/window/WinConsole.cpp -c -o ${OUTPUT_PATH}/WinConsole.o

clean:
	@echo "Cleaning Chip8"
	cmd //C del ./output/Chip8.exe
