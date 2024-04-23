#include <leer_pseudocodigo.h>

int leer_archivo(char *nombArch){
    char* archivo = PATH_INSTRUCCIONES;

    string_append(&archivo, nombArch);
    FILE* archivo_pseudo = fopen(archivo, "r");
    
    int programCounter = cantidad_instrucciones;

    char* instruccion = string_new();

    char* leido = fgetc(archivo_pseudo);
    string_append(&instruccion,&leido);
    while (!feof(archivo_pseudo))
    {
        while(leido != '\n'){
            leido= fgetc(archivo_pseudo);
            string_append(&instruccion,&leido);

        }
        printf("%s",instruccion);
        int h;
        h = 2;
        list_add_in_index(lista_de_instrucciones,cantidad_instrucciones,&instruccion);
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