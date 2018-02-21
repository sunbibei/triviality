all: 
	gcc -DPLAYER player.c     -o player
	gcc -DMASTER ringmaster.c -o ringmaster
debug:
	gcc -D__DEBUG__ -DPLAYER player.c     -o player
	gcc -D__DEBUG__ -DMASTER ringmaster.c -o ringmaster

player:
	gcc -DPLAYER player.c     -o player

ringmaster:
	gcc -DMASTER ringmaster.c -o ringmaster

clean:
	rm -f *~ *.o *.so player ringmaster

