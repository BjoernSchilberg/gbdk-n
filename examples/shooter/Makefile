BIN=../../bin
OBJ=./obj



build:
	mkdir -p $(OBJ)
	$(BIN)/gbdk-n-compile.sh shooter.c -o $(OBJ)/shooter.rel
	$(BIN)/gbdk-n-link.sh $(OBJ)/shooter.rel -o $(OBJ)/shooter.ihx
	$(BIN)/gbdk-n-make-rom.sh $(OBJ)/shooter.ihx shooter.gb

clean:
	rm -rf $(OBJ)
	rm -f shooter.gb
