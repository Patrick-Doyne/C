Compiler = gcc
compiles = add.c menu.c aliasLocation.c delete.c list.c lookup.c quit.c save.c update.c

create : $(compiles)
	$(Compiler) -o BSTtest -W -I../include $(compiles)

clean :
	rm -rf BSTtest
