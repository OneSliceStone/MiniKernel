# Variables de compilación
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -pthread
SRC = src/main.c src/queue.c src/cpu.c src/metrics.c
OBJ = $(SRC:.c=.o)
TARGET = minikernel

# Regla principal: compila el ejecutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Regla para compilar los archivos .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza de archivos generados
clean:
	rm -f src/*.o $(TARGET)