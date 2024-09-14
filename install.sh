#!/bin/bash

# Nombre del paquete requerido
PACKAGE_GROUP="Development Tools"

# Verifica si dnf está disponible
if command -v dnf &> /dev/null; then
    echo "Verificando e instalando dependencias..."
    
    # Instala el grupo de herramientas de desarrollo
    sudo dnf groupinstall -y "$PACKAGE_GROUP"
    
    if [ $? -eq 0 ]; then
        echo "Dependencias instaladas correctamente."
    else
        echo "Hubo un problema al instalar las dependencias. Revisa el log para más detalles."
        exit 1
    fi
else
    echo "dnf no está disponible en tu sistema. Por favor instala las dependencias manualmente."
    exit 1
fi

# Compilar el proyecto
echo "Compilando el proyecto..."
make

if [ $? -eq 0 ]; then
    echo "Compilación exitosa." 
    echo "Lea el README para más información sobre como correr la aplicación."
else
    echo "Hubo un problema durante la compilación. Revisa los mensajes de error."
    exit 1
fi

