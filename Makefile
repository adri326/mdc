CC_FLAGS += -Wall -Wextra
ifdef DEBUG
CC_FLAGS += -g
else
CC_FLAGS += -O3
endif

LINK_FLAGS += -lgmp
BUILD_DIR = build
SRC_DIR = src
INSTALL_DIR = ${HOME}/.local/bin

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/stack.c $(SRC_DIR)/calc.c
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

all: $(BUILD_DIR) $(BUILD_DIR)/mdc

install: $(BUILD_DIR)/mdc
	cp $(BUILD_DIR)/mdc $(INSTALL_DIR)/mdc

uninstall:
	[ -f $(INSTALL_DIR)/mdc ] && rm $(INSTALL_DIR)/mdc

clean:
	rm -f $(OBJ) $(BUILD_DIR)/mdc

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/mdc: $(BUILD_DIR) $(OBJ)
	$(CC) $(CC_FLAGS) $(LINK_FLAGS) $(OBJ) -o $@

$(OBJ): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -c $< -o $@
