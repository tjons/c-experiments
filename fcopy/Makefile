_cp:
	gcc cp.c -o "_cp"
	chmod +x ./_cp

_cp-debug:
	gcc cp.c -g -o "_cp-debug"
	chmod +x ./_cp-debug

test: _cp
	sh test.sh

clean:
	rm _cp
	rm test.txt
	rm copy.txt
