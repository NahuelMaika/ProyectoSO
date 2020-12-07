
struct message {
  long type;
};
typedef struct message tMessage;

#define FINISH "exit"
#define SIZE_MSG sizeof(tMessage) - sizeof(long)
#define FILE_PATH "constComidaRapida.h"
#define Tipo_Carne_Pedido 1L 
#define Tipo_Vegetal_Pedido 2L 
#define Tipo_Mesas 3L 

#define Tipo_Carne_Cocinar 1L 
#define Tipo_Vegetal_Cocinar 2L 

#define Tipo_Vegetal_Listo 4L
#define Tipo_Carne_Listo 5L
#define Tipo_Limpiador 6L 

#define Tipo_Carne 7L 
#define Tipo_Vegetal 8L 







void report_and_exit(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
