all: tareas miniShell leches leches2 comidaRapidaSema comidaRapidaColaDeMensajes

###########################Ej1 Tareas#########################################
tareas: EJ1_Tareas.o
	gcc -Wall -lpthread EJ1_Tareas.o -o tareas
EJ1_Tareas.o: EJ1_Tareas.c
	gcc -Wall -c -lpthread EJ1_Tareas.c

###########################MiniShell#######################################
miniShell: miniShell.o 
	gcc -Wall miniShell.o -o miniShell

miniShell.o: miniShell.c
	gcc -Wall -c miniShell.c

####################Demasiadas botellas de leche###########################
leches: EJ3.1_Leches.o
	gcc -Wall -lpthread EJ3.1_Leches.o -o leches
EJ3.1_Leches.o: EJ3.1_Leches.c
	gcc -Wall -c -lpthread EJ3.1_Leches.c

########################Ejercicio 2 Leches####################################
leches2: EJ3.2_Leches.o
	gcc -Wall EJ3.2_Leches.o -o leches2
EJ3.2_Leches.o: EJ3.2_Leches.c const_Leches.h
	gcc -Wall -c EJ3.2_Leches.c

#############Primer inciso de comida rapida con semaforos##################
comidaRapidaSema: comidaRapida.o
	gcc -Wall -lpthread comidaRapida.o -o comidaRapidaSema

ComidaRapida.o: comidaRapida.c
	gcc -Wall -lpthread -c comidaRapida.c

#############Inciso C con colas de mensajes###################3
comidaRapidaColaDeMensajes: ComidaRapida3.o
	gcc -Wall -lpthread ComidaRapida3.o -o comidaRapidaColaDeMensajes

ComidaRapida3.o: ComidaRapida3.c constComidaRapida.h
	gcc -Wall -c -lpthread ComidaRapida3.c

clean:
	rm -f *.o tareas miniShell leches leches2 comidaRapidaSema comidaRapidaColaDeMensajes