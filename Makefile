micro:
	clang micro.c -g -I libastro -force_load libastro/libastro.a -o micro.o
sample:
	clang sample.c -I libastro -force_load libastro/libastro.a -o sample.o
clean:
	rm -rf *.o	
