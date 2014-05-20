all:
	g++ -o ncnulocks ncnulocks.cpp -lncurses -lnulockscore
debug:
	g++ -o ncnulocks ncnulocks.cpp -lncurses -lnulockscore -g
install:
	cp ncnulocks /usr/bin
uninstall:
	rm /usr/bin/ncnulocks
clean:
	rm -rf ncnulocks
