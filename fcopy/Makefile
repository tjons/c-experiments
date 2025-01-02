_cp:
	gcc cp.c -o "_cp"
	chmod +x ./_cp

test: _cp
	sh test.sh

clean:
	rm _cp
	rm test.txt
	rm copy.txt
