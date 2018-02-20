all: 
	gcc player.c     -o player
	gcc ringmaster.c -o ringmaster

player:
	gcc player.c     -o player

ringmaster:
	gcc ringmaster.c -o ringmaster

clean:
	rm -f *~ *.o *.so player ringmaster

