
struct message {
  long type;
  char body[10];
};
typedef struct message tMessage;

#define FINISH "exit"
#define SIZE_MSG sizeof(tMessage) - sizeof(long)
#define FILE_PATH "constComidaRapida.h"
#define Tipo_Mesas 1L 
#define Tipo_Carne 2L 
#define Tipo_Vegetal 3L 
#define Tipo_Camarero 4L 
#define Tipo_Limpiador 5L 
#define Tipo_Cola_Comida 6L 




void report_and_exit(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
