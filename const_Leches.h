#define cant_procesos 10
#define FILE_PATH "const_Leches.h"
#define Tipo_Leche 1L //leche
#define Tipo_Compra 2L //comprarleche
#define Mutex_Lock 3L //Lock para la heladera

struct message{
	long type;
};
typedef struct message tMessage;

#define SIZE_MSG sizeof(struct message) - sizeof(long)

void report_and_exit(const char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}
