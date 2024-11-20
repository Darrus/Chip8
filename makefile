OUTPUT_PATH= .\build
OBJECTS= ${OUTPUT_PATH}\WinConsole.o ${OUTPUT_PATH}\Emulator.o ${OUTPUT_PATH}\Clock.o

all:${OBJECTS} 
	g++ .\src\main.cpp ${OBJECTS} -o ${OUTPUT_PATH}\Chip8.exe

${OUTPUT_PATH}\Emulator.o:src\Emulator.cpp
	g++ .\src\Emulator.cpp -c -o ${OUTPUT_PATH}\Emulator.o

${OUTPUT_PATH}\WinConsole.o:src\window\WinConsole.cpp
	g++ .\src\window\WinConsole.cpp -c -o ${OUTPUT_PATH}\WinConsole.o

${OUTPUT_PATH}\Clock.o:src\Clock.cpp
	g++ .\src\Clock.cpp -c -o ${OUTPUT_PATH}\Clock.o

clean:
	@echo "Cleaning Chip8"
	-del //f ${OUTPUT_PATH}\*.o ${OUTPUT_PATH}\*.exe
