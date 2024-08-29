a.out:client.o menu.o 
	gcc *.o -o $@
%.o:%.c
	gcc -c $< -o $@
clean:
	rm -rf *.o a.out