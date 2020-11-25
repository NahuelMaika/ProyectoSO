
struct message {
  long type;
  char body[20];
};
typedef struct message tMessage;

#define FINISH "exit"
#define SIZE_MSG sizeof(tMessage) - sizeof(long)
#define FILE_PATH "constComidaRapida.h"
#define TYPE_1 1L //mesas disponibles
#define TYPE_2 2L // pedir un plato
#define TYPE_3 3L // recibir plato
#define TYPE_4 4L // ordenar que limpien la mesa
#define TYPE_5 5L //ordenar al chef que haga un plato
#define TYPE_6 6L //chef avisa que tiene listo el plato
#define TYPE_7 7L
#define TYPE_8 8L
#define TYPE_9 9L



void report_and_exit(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
