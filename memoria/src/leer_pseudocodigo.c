#include <leer_pseudocodigo.h>

int leer_archivo(char *nombArch, t_dictionary* diccionario,int pid){
    pthread_mutex_lock(&mutex_para_leer_pseudo);
    char * archivo = strdup(PATH_INSTRUCCIONES);
    string_append(&archivo, nombArch);
    FILE* archivo_pseudo = fopen(archivo, "r");
    if(archivo_pseudo != NULL){
        int programCounter = 0;
        char* instruccion = string_new();
        t_list* lista_de_intrucciones = list_create();
        while (!feof(archivo_pseudo))
        {
        
            fgets(instruccion,256,archivo_pseudo);
            char* a_guardar = strdup(instruccion);

            list_add(lista_de_intrucciones,a_guardar);

            free(instruccion);
            instruccion = string_new();

        
        }
        char* clave_pid = string_itoa(pid);
        dictionary_put(diccionario_de_instrucciones,clave_pid,lista_de_intrucciones);
        fclose(archivo_pseudo);
        pthread_mutex_unlock(&mutex_para_leer_pseudo);
        return programCounter;
    }
    else{
       pthread_mutex_unlock(&mutex_para_leer_pseudo);
       return -1; 
    }
    
    
}