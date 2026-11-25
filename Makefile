CC = arm-none-eabi-gcc
BUILD_DIR = output
NWLINK = nwlink

src = src/main.c
CFLAGS = -std=c99 $(shell $(NWLINK) eadk-cflags) -Os -Wall -Wextra
LDFLAGS = -Wl,--relocatable -nostartfiles --specs=nano.specs
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,-e,main -Wl,-u,eadk_app_name -Wl,-u,eadk_app_icon -Wl,-u,eadk_api_level -Wl,--gc-sections

.PHONY: build run clean
build: $(BUILD_DIR)/PHYSIX.nwa

$(BUILD_DIR)/main.o: $(src)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/icon.o: src/icon.png
	$(NWLINK) png-icon-o $< $@

$(BUILD_DIR)/PHYSIX.nwa: $(BUILD_DIR)/main.o $(BUILD_DIR)/icon.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

run: build
	$(NWLINK) install-nwa $(BUILD_DIR)/PHYSIX.nwa

clean:
	rm -rf $(BUILD_DIR)
