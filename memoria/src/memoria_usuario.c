#include <memoria_usuario.h>

int cambiar_memoria_de_proceso(int pid, int nuevo_tam){ //SE TIENE QUE SEGUIR DESARROLLANDO
    char* clave_pid = string_itoa(pid);

    pthread_mutex_lock(&mutex_para_diccionario_tdp);
    t_list* tdp_del_proceso = dictionary_get(diccionario_de_tdp,clave_pid);
    pthread_mutex_unlock(&mutex_para_diccionario_tdp);

    int tam_actual = list_size(tdp_del_proceso) * TAM_PAGINA;

    if(tam_actual > nuevo_tam){
    
    }
    else if(tam_actual < nuevo_tam){
        int bytesNecesarios = nuevo_tam - tam_actual;
    }

    return true;
}