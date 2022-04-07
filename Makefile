CC_FLAGS += -Wall -Wextra -g
LINK_FLAGS += -lgmp
BUILD_DIR = build
SRC_DIR = src
INSTALL_DIR = ${HOME}/.local/bin

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/stack.c
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

all: $(BUILD_DIR) $(BUILD_DIR)/mdc

install: $(BUILD_DIR)/mdc
	cp $(BUILD_DIR)/mdc $(INSTALL_DIR)/mdc

uninstall:
	[ -f $(INSTALL_DIR)/mdc ] && rm $(INSTALL_DIR)/mdc

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/mdc: $(BUILD_DIR) $(OBJ)
	$(CC) $(CC_FLAGS) $(LINK_FLAGS) $(OBJ) -o $@

$(OBJ): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -c $< -o $@
