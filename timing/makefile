CC = g++
CFLAGS = -O2 -std=c++11 -pthread -march=native
LIBS = -lntl -lmpfr -lgmp

TARGET1 = crc
SRCS1 = CompositeResiduosity.cpp

TARGET2 = fdv
SRCS2 = FastDecryptionVariant.cpp

BUILD_DIR = build
OBJS1 = $(BUILD_DIR)/$(SRCS1:.cpp=.o)
OBJS2 = $(BUILD_DIR)/$(SRCS2:.cpp=.o)

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(TARGET1) $(TARGET2) $(BUILD_DIR)