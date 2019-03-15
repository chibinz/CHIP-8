gcc -c main.c
gcc -c cpu.c
gcc -c ram.c
gcc -c rom.c
gcc -c graphics.c
gcc -c interpret.c
gcc -c disassemble.c
gcc -c window.c
gcc -c shader.c
gcc -c glad.c

gcc -o main.exe main.o cpu.o ram.o rom.o graphics.o interpret.o disassemble.o window.o shader.o glad.o -lopengl32 -lglfw3dll

.\main.exe IBM.ch8