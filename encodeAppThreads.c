#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>

#define MAX_PATH_LENGTH 1024
struct threadArg{
  struct dirent *entrada;
  char *argv;
};

void *threadFunction(void *arg){
    struct threadArg *targ = (struct threadArg *)arg;
    char fileName[MAX_PATH_LENGTH];
    char outputFileName[MAX_PATH_LENGTH];
    char command[MAX_PATH_LENGTH * 3];
    if (strstr(targ->entrada->d_name, ".txt") != NULL) {
          // Preparar los nombres de archivo y de salida
          snprintf(fileName, sizeof(fileName), "%s/%s", targ->argv, targ->entrada->d_name);
          snprintf(outputFileName, sizeof(outputFileName), "%s.bin", targ->entrada->d_name);

          // Construir el comando
          snprintf(command, sizeof(command), "./encode %s %s", fileName, outputFileName);
          // Ejecutar el comando
          int returnCode = system(command);

          // Verificar el código de retorno
          if (returnCode != 0) {
              fprintf(stderr, "Error procesando el archivo: %s\n", targ->entrada->d_name);
          }
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    struct timespec start, end;
    long long int nano_start, nano_end;
    double tiempo_en_segundos;

    clock_gettime(CLOCK_REALTIME, &start);
    
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <directorio>\n", argv[0]);
        return 1;
    }

    DIR *directorio;
    struct dirent *entrada;
    /*char fileName[MAX_PATH_LENGTH];
    char outputFileName[MAX_PATH_LENGTH];
    char command[MAX_PATH_LENGTH * 3];*/ // Tamaño suficiente para el comando

    if ((directorio = opendir(argv[1])) == NULL) {
        perror("No se puede abrir el directorio");
        return EXIT_FAILURE;
    }
    int fileCount = 0;
    while ((entrada = readdir(directorio)) != NULL) {
        fileCount++;
    }
    closedir(directorio);
    
    if ((directorio = opendir(argv[1])) == NULL) {
        perror("No se puede abrir el directorio");
        return EXIT_FAILURE;
    }
    
    pthread_t tid[fileCount];
    // Procesar archivos .txt en el directorio
    for(int t = 0; t < fileCount; t++){
      struct threadArg *tar = malloc(sizeof(struct threadArg *));
      tar->argv = argv[1];
      tar->entrada = readdir(directorio);
      pthread_create(&tid[t], NULL, (void *)threadFunction, (void *)tar);
    }
    //while ((entrada = readdir(directorio)) != NULL) {
        // Verificar si el archivo tiene extensión .txt
        /*if (strstr(entrada->d_name, ".txt") != NULL) {
            // Preparar los nombres de archivo y de salida
            snprintf(fileName, sizeof(fileName), "%s/%s", argv[1], entrada->d_name);
            snprintf(outputFileName, sizeof(outputFileName), "%s.bin", entrada->d_name);

            // Construir el comando
            snprintf(command, sizeof(command), "./encode %s %s", fileName, outputFileName);

            // Ejecutar el comando
            int returnCode = system(command);

            // Verificar el código de retorno
            if (returnCode != 0) {
                fprintf(stderr, "Error procesando el archivo: %s\n", entrada->d_name);
            }
        }*/
    //}
    for (int t = 0; t < fileCount; t++) {
        pthread_join(tid[t], NULL);
    }
    FILE *outputFile = fopen("comprimido.bin", "wb");
    
    if (outputFile == NULL) {
        perror("Error abriendo el archivo de salida");
        return 1;
    }

    if ((directorio = opendir(".")) == NULL) {
        perror("No se puede abrir el directorio");
        fclose(outputFile);
        return 1;
    }

    // Procesar archivos .bin en el directorio
    while ((entrada = readdir(directorio)) != NULL) {
        // Verificar si el archivo tiene extensión .bin
        if (strstr(entrada->d_name, ".bin") != NULL && strcmp(entrada->d_name, "comprimido.bin") != 0) {
            FILE *inputFile = fopen(entrada->d_name, "rb");
            if (inputFile == NULL) {
                printf("Error abriendo el archivo de entrada: %s\n", entrada->d_name);
                continue;
            }

            //printf("Combinando archivo: %s\n", entrada->d_name);

            // Escribir metadatos: Nombre del archivo y su longitud
            unsigned char fileNameLength = (unsigned char)strlen(entrada->d_name);
            fwrite(&fileNameLength, sizeof(unsigned char), 1, outputFile);  // Escribir longitud del nombre del archivo
            fwrite(entrada->d_name, sizeof(char), fileNameLength, outputFile);      // Escribir el nombre del archivo

            // Leer el contenido del archivo de entrada y escribirlo en el archivo de salida
            char buffer[1024];
            size_t bytesRead;
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0) {
                fwrite(buffer, 1, bytesRead, outputFile);
            }

            fclose(inputFile);

            // Agregar separador entre archivos (por ejemplo, 4 bytes con valor 0xFF)
            unsigned char separator[4] = {0xFF, 0xFF, 0xFF, 0xFF};
            fwrite(separator, sizeof(unsigned char), 4, outputFile);

            // Eliminar el archivo después de combinarlo
            if (remove(entrada->d_name) != 0) {
                perror("Error al eliminar el archivo");
            } else {
                //printf("Archivo %s eliminado.\n", entrada->d_name);
            }
        }
    }

    closedir(directorio);
    fclose(outputFile);

    printf("Todos los archivos .bin combinados en comprimido.bin\n");

    clock_gettime(CLOCK_REALTIME, &end);
    // Convierte los tiempos a nanosegundos desde el inicio de la época
    nano_start = start.tv_sec * 1000000000LL + start.tv_nsec;
    nano_end = end.tv_sec * 1000000000LL + end.tv_nsec;

    // Calcula la diferencia en nanosegundos
    long long int tiempo_en_nanosegundos = nano_end - nano_start;

    // Convierte nanosegundos a segundos
    tiempo_en_segundos = tiempo_en_nanosegundos / 1000000000.0;

    // Imprime los resultados
    printf("Tiempo transcurrido: %lld nanosegundos\n", tiempo_en_nanosegundos);
    printf("Tiempo transcurrido: %.9f segundos\n", tiempo_en_segundos);

    return 0;
}
