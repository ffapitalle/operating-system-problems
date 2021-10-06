all: sleeping_barber

sleeping_barber: sleeping_barber.c
	gcc -lpthread -o sleeping_barber sleeping_barber.c
