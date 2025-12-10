CC = gcc
INC = -Iinclude
SRC_DIR = src
BUILD_DIR = build
OPTIMIZATION = -O2

.PHONY: clean

%:
	@mkdir -p $(BUILD_DIR)
	@if [ ! -f $(SRC_DIR)/$@.c ]; then \
		echo "Error: $(SRC_DIR)/$@.c not found"; exit 1; \
	fi
	@echo "day $@:"
	@$(CC) $(INC) $(OPTIMIZATION) $(SRC_DIR)/$@.c -o $(BUILD_DIR)/$@.out
	@./$(BUILD_DIR)/$@.out

clean:
	rm -rf $(BUILD_DIR)/*.out
