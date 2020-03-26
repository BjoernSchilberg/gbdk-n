BIN=../../bin
OBJ=./obj



build:
	mkdir -p $(OBJ)
	$(BIN)/gbdk-n-compile.sh adventure.c -o $(OBJ)/adventure.rel
	$(BIN)/gbdk-n-link.sh $(OBJ)/adventure.rel -o $(OBJ)/adventure.ihx
	$(BIN)/gbdk-n-make-rom.sh $(OBJ)/adventure.ihx adventure.gb

clean:
	rm -rf $(OBJ)
	rm -f adventure.gb
