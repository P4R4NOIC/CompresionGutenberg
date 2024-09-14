#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>

#define MAX_PATH_LENGTH 256
#define SEPARATOR_SIZE 4

void *threadFunction(void *arg){
    struct dirent *entrada = (struct dirent *)arg;
    char filePath[MAX_PATH_LENGTH];
    char command[MAX_PATH_LENGTH * 2];
    
    if (strstr(entrada->d_name, ".bin") != NULL && strcmp(entrada->d_name, "comprimido.bin") != 0) {
            // Preparar la ruta completa del archivo
        snprintf(filePath, sizeof(filePath), "%s", entrada->d_name);

        // Construir el comando para ejecutar decode
        snprintf(command, sizeof(command), "./decode %s", filePath);

        // Ejecutar el comando
        int returnCode = system(command);

        // Verificar el código de retorno
        if (returnCode != 0) {
            fprintf(stderr, "Error procesando el archivo: %s\n", entrada->d_name);
        }

        // Borrar el archivo después de procesarlo
        if (remove(filePath) != 0) {
            perror("Error al eliminar el archivo");
        } else {
            //printf("Archivo %s eliminado exitosamente.\n", filePath);
        }
    }
    return NULL;
    
}

// Función auxiliar para buscar un separador en un buffer
int find_separator(unsigned char *buffer, size_t buffer_size, unsigned char *separator, size_t sep_size) {
    for (size_t i = 0; i <= buffer_size - sep_size; i++) {
        if (memcmp(buffer + i, separator, sep_size) == 0) {
            return i;
        }
    }
    return -1;
}

// Función para extraer archivos del archivo combinado
void extract_files(FILE *inputFile) {
    unsigned char separator[SEPARATOR_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF};
    unsigned char fileNameLength;
    char fileName[MAX_PATH_LENGTH];
    FILE *outputFile;

    while (fread(&fileNameLength, sizeof(unsigned char), 1, inputFile) == 1) {
        if (fread(fileName, sizeof(char), fileNameLength, inputFile) != fileNameLength) {
            perror("Error reading file name");
            exit(1);
        }
        fileName[fileNameLength] = '\0';

        // Verificar si el nombre del archivo termina con la extensión .txt.bin
        if (!strstr(fileName, ".txt.bin")) {
            
            // Leer el resto del archivo hasta el siguiente separador
            unsigned char buffer[1024];
            size_t bytesRead;
            int separatorPos;

            while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0) {
                separatorPos = find_separator(buffer, bytesRead, separator, SEPARATOR_SIZE);
                if (separatorPos >= 0) {
                    fseek(inputFile, separatorPos - bytesRead + SEPARATOR_SIZE, SEEK_CUR);
                    break;
                }
            }
            continue;
        }

        //printf("Extracting file: %s\n", fileName);
        outputFile = fopen(fileName, "wb");
        if (outputFile == NULL) {
            perror("Error creating output file");
            exit(1);
        }

        unsigned char buffer[1024];
        size_t bytesRead;
        int foundSeparator = 0;

        while ((bytesRead = fread(buffer, 1, sizeof(buffer), inputFile)) > 0) {
            int separatorPos = find_separator(buffer, bytesRead, separator, SEPARATOR_SIZE);

            if (separatorPos >= 0) {
                fwrite(buffer, 1, separatorPos, outputFile);
                foundSeparator = 1;
                fseek(inputFile, separatorPos - bytesRead + SEPARATOR_SIZE, SEEK_CUR);
                break;
            } else {
                fwrite(buffer, 1, bytesRead, outputFile);
            }

        }

        fclose(outputFile);
        if (!foundSeparator) {
            printf("No separator found for file %s.\n", fileName);
            exit(1);
        }

    }
}

// Función para renombrar archivos eliminando dos extensiones adicionales
void rename_files() {
    DIR *directorio;
    struct dirent *entrada;
    char oldName[MAX_PATH_LENGTH];
    char newName[MAX_PATH_LENGTH];
    char *lastDot;

    if ((directorio = opendir(".")) == NULL) {
        perror("No se puede abrir el directorio");
        exit(1);
    }

    while ((entrada = readdir(directorio)) != NULL) {
        // Verificar si el archivo tiene la extensión adicional .txt.bin.txt
        if (strstr(entrada->d_name, ".txt.bin.txt") != NULL) {
            // Preparar los nombres antiguo y nuevo
            snprintf(oldName, sizeof(oldName), "%s", entrada->d_name);

            // Eliminar las dos últimas extensiones ('.bin' y '.txt')
            lastDot = strrchr(oldName, '.');
            if (lastDot != NULL && strcmp(lastDot, ".txt") == 0) {
                *lastDot = '\0'; // Eliminar '.txt'
                lastDot = strrchr(oldName, '.');
                if (lastDot != NULL && strcmp(lastDot, ".bin") == 0) {
                    *lastDot = '\0'; // Eliminar '.bin'
                }
            }

            snprintf(newName, sizeof(newName), "%s", oldName);

            // Renombrar el archivo
            if (rename(entrada->d_name, newName) != 0) {
                perror("Error renaming file");
            } else {
                //printf("File renamed from %s to %s\n", entrada->d_name, newName);
            }
        }
    }

    closedir(directorio);
}

int main(int argc, char *argv[]) {
    struct timespec start, end;
    long long int nano_start, nano_end;
    double tiempo_en_segundos;

    clock_gettime(CLOCK_REALTIME, &start);

    if (argc != 2) {
        printf("Usage: %s <combined-bin-file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "rb");
    if (inputFile == NULL) {
        perror("Error opening combined file");
        return 1;
    }
    

    extract_files(inputFile);

    fclose(inputFile);

    DIR *directorio;
    struct dirent *entrada;
    
    
    // Abrir el directorio actual
    if ((directorio = opendir(".")) == NULL) {
        perror("No se puede abrir el directorio");
        return 1;
    }
    
    
    int fileCount = 0;
    while ((entrada = readdir(directorio)) != NULL) {
        fileCount++;
    }
    closedir(directorio);
    
    if ((directorio = opendir(".")) == NULL) {
        perror("No se puede abrir el directorio");
        return EXIT_FAILURE;
    }
    pthread_t tid[fileCount];
    
    // Iterar sobre todos los archivos en el directorio
    for(int t = 0; t < fileCount; t++){
      entrada = readdir(directorio);
      pthread_create(&tid[t], NULL, (void *)threadFunction, (void *)entrada);
    }
    // Iterar sobre todos los archivos en el directorio
    /*while ((entrada = readdir(directorio)) != NULL) {
        // Verificar si el archivo tiene extensión .bin y no es 'comprimido.bin'
        if (strstr(entrada->d_name, ".bin") != NULL && strcmp(entrada->d_name, "comprimido.bin") != 0) {
            // Preparar la ruta completa del archivo
            snprintf(filePath, sizeof(filePath), "%s", entrada->d_name);

            // Construir el comando para ejecutar decode
            snprintf(command, sizeof(command), "./decode %s", filePath);

            // Ejecutar el comando
            int returnCode = system(command);

            // Verificar el código de retorno
            if (returnCode != 0) {
                fprintf(stderr, "Error procesando el archivo: %s\n", entrada->d_name);
            }

            // Borrar el archivo después de procesarlo
            if (remove(filePath) != 0) {
                perror("Error al eliminar el archivo");
            } else {
                //printf("Archivo %s eliminado exitosamente.\n", filePath);
            }
        }
    }*/
    for (int t = 0; t < fileCount; t++) {
        pthread_join(tid[t], NULL);
    }
    // Cerrar el directorio
    closedir(directorio);

    // Renombrar archivos con extensiones adicionales
    rename_files();

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
