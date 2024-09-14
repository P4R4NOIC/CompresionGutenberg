# Makefile para compilar compresion.c, descompresion.c, encodeApp.c, decodeApp.c, encodeAppFork.c y decodeAppFork.c

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -g

# Nombres de los ejecutables
EXE_COMPRESION = encode
EXE_DESCOMPRESION = decode
EXE_ENCODEAPP = encodeApp
EXE_DECODEAPP = decodeApp
EXE_ENCODEAPPFORK = encodeAppFork
EXE_DECODEAPPFORK = decodeAppFork
EXE_ENCODEAPPTHREADS = encodeAppThreads
EXE_DECODEAPPTHREADS = decodeAppThreads

# Objetivos (targets)
all: $(EXE_COMPRESION) $(EXE_DESCOMPRESION) $(EXE_ENCODEAPP) $(EXE_DECODEAPP) $(EXE_ENCODEAPPFORK) $(EXE_DECODEAPPFORK) $(EXE_ENCODEAPPTHREADS) $(EXE_DECODEAPPTHREADS)

# Regla para compilar el ejecutable de encode
$(EXE_COMPRESION): encode.o
	$(CC) $(CFLAGS) -o $(EXE_COMPRESION) encode.o

# Regla para compilar el ejecutable de descompresion
$(EXE_DESCOMPRESION): decode.o
	$(CC) $(CFLAGS) -o $(EXE_DESCOMPRESION) decode.o

# Regla para compilar el ejecutable de encodeApp
$(EXE_ENCODEAPP): encodeApp.o
	$(CC) $(CFLAGS) -o $(EXE_ENCODEAPP) encodeApp.o

# Regla para compilar el ejecutable de decodeApp
$(EXE_DECODEAPP): decodeApp.o
	$(CC) $(CFLAGS) -o $(EXE_DECODEAPP) decodeApp.o

# Regla para compilar el ejecutable de encodeAppFork
$(EXE_ENCODEAPPFORK): encodeAppFork.o
	$(CC) $(CFLAGS) -o $(EXE_ENCODEAPPFORK) encodeAppFork.o

# Regla para compilar el ejecutable de decodeAppFork
$(EXE_DECODEAPPFORK): decodeAppFork.o
	$(CC) $(CFLAGS) -o $(EXE_DECODEAPPFORK) decodeAppFork.o
	
# Regla para compilar el ejecutable de encodeAppThreads
$(EXE_ENCODEAPPTHREADS): encodeAppThreads.o
	$(CC) $(CFLAGS) -o $(EXE_ENCODEAPPTHREADS) encodeAppThreads.o

# Regla para compilar el ejecutable de decodeAppThreads
$(EXE_DECODEAPPTHREADS): decodeAppThreads.o
	$(CC) $(CFLAGS) -o $(EXE_DECODEAPPTHREADS) decodeAppThreads.o
	
# Regla para compilar el archivo objeto de encode.c
encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

# Regla para compilar el archivo objeto de decode.c
decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

# Regla para compilar el archivo objeto de encodeApp.c
encodeApp.o: encodeApp.c
	$(CC) $(CFLAGS) -c encodeApp.c

# Regla para compilar el archivo objeto de decodeApp.c
decodeApp.o: decodeApp.c
	$(CC) $(CFLAGS) -c decodeApp.c

# Regla para compilar el archivo objeto de encodeAppFork.c
encodeAppFork.o: encodeAppFork.c
	$(CC) $(CFLAGS) -c encodeAppFork.c

# Regla para compilar el archivo objeto de decodeAppFork.c
decodeAppFork.o: decodeAppFork.c
	$(CC) $(CFLAGS) -c decodeAppFork.c

# Regla para compilar el archivo objeto de encodeAppThreads.c
encodeAppThreads.o: encodeAppThreads.c
	$(CC) $(CFLAGS) -c encodeAppThreads.c

# Regla para compilar el archivo objeto de decodeAppThreads.c
decodeAppThreads.o: decodeAppThreads.c
	$(CC) $(CFLAGS) -c decodeAppThreads.c

# Limpiar archivos generados
clean:
	rm -f *.o $(EXE_COMPRESION) $(EXE_DESCOMPRESION) $(EXE_ENCODEAPP) $(EXE_DECODEAPP) $(EXE_ENCODEAPPFORK) $(EXE_DECODEAPPFORK) $(EXE_ENCODEAPPTHREADS) $(EXE_DECODEAPPTHREADS)