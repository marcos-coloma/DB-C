# DB-C

Este es un proyecto de aprendizaje hecho en C

Un motor de base de datos relacional ligero, diseñado para el almacenamiento persistente de registros en formato binario. El proyecto implementa la gestión manual de memoria, manipulación de archivos a bajo nivel y una arquitectura desacoplada entre el nucleo (core) y la interfaz de usuario (CLI).

Está dividido en dos partes:

- lib/ → Librería que contiene toda la lógica interna de la base de datos.
- app/ → Aplicación de línea de comandos (CLI) que permite al usuario interactuar con la base de datos mediante comandos.

Para compilar el proyecto:
- mingw32-make

Para ejecutar la aplicación:
- ./build/db_test

nota: si no podes compilar o ejecutar, fijate el makefile

# Comandos Disponibles:
- help Lista los comandos y ayuda del sistema.
- table list	Lista todas las tablas detectadas en el sistema.
- table create <name>	Crea una nueva tabla física y en memoria.
- table drop <name>	Elimina la tabla y su archivo asociado.
- record insert <fields...>	Inserta un nuevo registro en la tabla activa.
- record read <index>	Recupera un registro específico por su posición.
- record update <index> <fields...>	Modifica los datos de un registro existente.
- record delete <index> Marca un registro para su eliminación y libera su acceso.
- exit Cierra la base de datos y finaliza la sesión.

