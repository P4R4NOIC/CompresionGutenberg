# Compresión Gutenberg

Este proyecto contiene múltiples programas de compresión y descompresión que utilizan el algoritmo de Huffman.

## Autores
- Dylan Mora Corrales
- Fernando Gross Hernández
- Roberto Vindas Hernández

## Instalación

Para instalar los ejecutables, siga estos pasos:

1. Da permisos de ejecución al script de instalación:
    ```bash
    chmod +x install.sh
    ```

2. Ejecuta el script de instalación:
    ```bash
    ./install.sh
    ```

## Uso

### Compresión

Para comprimir archivos en un directorio, utilice uno de los siguientes comandos:

- Para compresión básica:
    ```bash
    ./encodeApp <directorio>
    ```
- Para compresión con procesos (fork):
    ```bash
    ./encodeAppFork <directorio>
    ```
- Para compresión con hilos (threads):
    ```bash
    ./encodeAppThreads <directorio>
    ```

### Descompresión

Para descomprimir el archivo binario, utilice uno de los siguientes comandos:

- Para descompresión básica:
    ```bash
    ./decodeApp comprimido.bin
    ```
- Para descompresión con procesos (fork):
    ```bash
    ./decodeAppFork comprimido.bin
    ```
- Para descompresión con hilos (threads):
    ```bash
    ./decodeAppThreads comprimido.bin
    ```

**Nota:** Antes de usar cualquier descompresor, asegúrese de copiar los siguientes archivos en la carpeta donde desea descomprimir los archivos:
1. El ejecutable `decodeApp` a utilizar.
2. El ejecutable `decode`.
3. El archivo `comprimido.bin`.
