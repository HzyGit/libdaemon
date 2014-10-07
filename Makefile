CC:=g++
.PHONY: clean
test: test.o daemon.o
test.o: daemon.h
dameon.o: daemon.h

init:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
	cscope -bqR
clean:
	-rm *.o
	-rm test
	-rm tags
	-rm cscope.in.out
	-rm cscope.out
	-rm cscope.po.out
