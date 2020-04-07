PROG=thumby

GBDK_N = ~/gbdk-n
CC = $(GBDK_N)/bin/gbdk-n-compile.sh
LD = $(GBDK_N)/bin/gbdk-n-link.sh
MR = $(GBDK_N)/bin/gbdk-n-make-rom.sh
OBJ=./obj

.PHONY: all format build clean

all: format build

format:
	@echo "Formatting ..."
	astyle \
	--style=1tbs \
	--lineend=linux \
	--convert-tabs \
	--preserve-date \
	--pad-header \
	--indent-switches \
	--align-pointer=name \
	--align-reference=name \
	--pad-oper \
	--suffix=none \
	$(PROG).c
build:
	@echo "Building ..."
	mkdir -p $(OBJ)
	@echo "Compile ..."
	$(CC) $(PROG).c -o $(OBJ)/$(PROG).rel
	@echo "Link ..."
	$(LD) $(OBJ)/$(PROG).rel -o $(OBJ)/$(PROG).ihx
	@echo "Make ROM ..."
	$(MR) $(OBJ)/$(PROG).ihx $(PROG).gb

clean:
	@echo "Cleaning ..."
	rm -rf $(OBJ)
	rm -f $(PROG).gb
	rm -f $(PROG).sav
run:
	@echo "Cool running ..."
	mgba $(PROG).gb
