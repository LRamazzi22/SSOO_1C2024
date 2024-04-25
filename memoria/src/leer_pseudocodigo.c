#include <leer_pseudocodigo.h>

int leer_archivo(char *nombArch, t_dictionary* diccionario){
    pthread_mutex_lock(&mutex_para_leer_pseudo);
    char * archivo = strdup(PATH_INSTRUCCIONES);
    string_append(&archivo, nombArch);
    FILE* archivo_pseudo = fopen(archivo, "r");
    int programCounter = cantidad_instrucciones;
    char* instruccion = string_new();
    while (!feof(archivo_pseudo))
    {
        
        fgets(instruccion,256,archivo_pseudo);
        char* a_guardar = strdup(instruccion);
        
        char num [2];
        sprintf(num,"%d",cantidad_instrucciones);
        
        dictionary_put(diccionario,num,a_guardar);
        cantidad_instrucciones++;
        free(instruccion);
        instruccion = string_new();

        
    }
    fclose(archivo_pseudo);
    pthread_mutex_unlock(&mutex_para_leer_pseudo);
    return programCounter;
    
}