TARGET := proyecto3

SOURCE_DIR  := ./src
INCLUDE_DIR := ./include
BUILD_DIR   := ./build
TEST_DIR    := ./test

CC        := gcc
CFLAGS    := -Wall -c
LDFLAGS   := -lm -lpthread `sdl2-config --libs --cflags` -lSDL2 -lSDL2_image -lSDL2_ttf
INC_FLAGS := -I$(INCLUDE_DIR)

SOURCES 		 = $(wildcard $(SOURCE_DIR)/*.c)
HEADERS 		 = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS 		 = $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)
TESTS   		 = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(TESTS:$(TEST_DIR)/%.c=$(BUILD_DIR)/test/%.o)


.PHONY: all
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/test/$(TARGET)


$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

$(BUILD_DIR)/test/$(TARGET): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(TEST_OBJECTS) $(filter-out $(BUILD_DIR)/threadville_main.o, $(OBJECTS)) -o $@ $(LDFLAGS)

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@
	mkdir -p $@/test

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
