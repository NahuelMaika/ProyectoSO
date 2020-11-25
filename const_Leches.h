#define cant_procesos 15
#define FILE_PATH "/home/pi/Desktop/Proyecto"
#define Tipo1 1L //leche
#define Tipo2 2L //comprarleche
#define Tipo3 3L

struct message{
	long type;
};
typedef struct message tMessage;

#define SIZE_MSG sizeof(struct message) - sizeof(long)

void report_and_exit(const char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}
