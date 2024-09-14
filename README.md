### Primero, da permisos de ejecución al script:
chmod +x install.sh

### Luego, ejecuta el script:
./install.sh

## Para utilizar el código correctamente, debe realizar lo siguiente:
### Al usar un compresor, escribalo de la siguiente manera
./encodeApp <directorio>
./encodeAppFork <directorio>
./encodeAppThreads <directorio>

## Al usar el descompresor
./decodeApp comprimido.bin
./decodeAppFork comprimido.bin
./decodeAppThreads comprimido.bin
## Nota: Al usar cualquier descompresor, debe de copiar los siguientes 3 archivos en la carpeta donde desea descomprimir los archivos:
### 1. El ejecutable decodeApp a utilizar
### 2. El ejecutable decode
### 3. El comprimido.bin

