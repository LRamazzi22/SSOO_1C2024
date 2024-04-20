#include <leer_pseudocodigo.h>

int leer_archivo(char *nombArch){
    char * archivo = PATH_INSTRUCCIONES;
    int contador = 0;

    string_append(&archivo, nombArch);
    FILE* archivo_pseudo = fopen(archivo, "r");
    
    char * instruccion = string_new();
    char* leido = "";
    int programCounter = instrucciones->cantidad_instrucciones;
    while (feof(archivo_pseudo) != 0)
    {
        do{
           fread(leido, sizeof(char),1, archivo_pseudo);
           string_append(&instruccion,leido);  
        } while (*leido != 10);
        
        strcpy(instrucciones->lista_de_instrucciones[instrucciones->cantidad_instrucciones + contador],instruccion);
        contador++;
        instrucciones->cantidad_instrucciones++;
    }

    fclose(archivo_pseudo);
    return programCounter;
}