CC = gcc
TARGET = main
IMAGE = image.ppm
SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)

CFLAGS = -O3 -march=native -flto -Wall -Wextra -I$(SRC_DIR) -lm 
LDFLAGS = -flto

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CFLAGS += -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
	LDFLAGS += -L/opt/homebrew/opt/libomp/lib -lomp
	VIEW_CMD = open
else
	CFLAGS += -fopenmp
	LDFLAGS += -fopenmp
	VIEW_CMD = xdg-open 
endif

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	time ./$(TARGET) > $(IMAGE)
	$(VIEW_CMD) $(IMAGE)

clean:
	rm -f $(TARGET) $(IMAGE)

.PHONY: all run clean
