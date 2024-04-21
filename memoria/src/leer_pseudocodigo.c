#include <leer_pseudocodigo.h>

int leer_archivo(char *nombArch){
    char * archivo = PATH_INSTRUCCIONES;

    string_append(&archivo, nombArch);
    FILE* archivo_pseudo = fopen(archivo, "r");
    
    char * instruccion = string_new();
    int programCounter = cantidad_instrucciones;

    char leido = fgetc(archivo_pseudo);
    string_append(&instruccion,&leido);
    while (!feof(archivo_pseudo))
    {
        while(leido != '\n'){
            leido= fgetc(archivo_pseudo);
            string_append(&instruccion,&leido);

        }
        
        
        lista_de_instrucciones = realloc(lista_de_instrucciones, (cantidad_instrucciones+1)*sizeof(char*));
        lista_de_instrucciones[cantidad_instrucciones] = malloc(strlen(instruccion)+1);
        memcpy(lista_de_instrucciones[cantidad_instrucciones], instruccion,strlen(instruccion)+1);
        cantidad_instrucciones++;
        
        leido= fgetc(archivo_pseudo);
        free(instruccion);
        instruccion = string_new();
        string_append(&instruccion,&leido);
        
        
    }
    free(instruccion);
    free(archivo);
    fclose(archivo_pseudo);
    return programCounter;
}