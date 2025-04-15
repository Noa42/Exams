#include <unistd.h>     // Para fork(), execve(), pipe(), dup(), dup2()
#include <sys/wait.h>   // Para waitpid()
#include <string.h>     // Para strcmp()
#include <stdlib.h>     // Para exit()
#include <stdio.h>      // Para printf() (usado solo en comentarios explicativos)

/* ============================
 *  FUNCION AUXILIAR: ERROR STDERR
 * ============================ */

// Imprime un mensaje de error en stderr
void ft_putstr_fd2(char *str, char *arg) {
    while (*str)
        write(2, str++, 1); // Escribe en STDERR
    if (arg)
        while (*arg)
            write(2, arg++, 1);
    write(2, "\n", 1); // Nueva línea
}

/* ============================
 *  FUNCION PRINCIPAL DE EJECUCIÓN
 * ============================ */

// Ejecuta un comando en un proceso hijo
void ft_execute(char *argv[], int i, int tmp_fd, char *env[]) {
    argv[i] = NULL; // Marca el final del array de argumentos
    dup2(tmp_fd, STDIN_FILENO); // Redirige la entrada estándar (para pipes)
    close(tmp_fd); // Cierra el descriptor antiguo
    execve(argv[0], argv, env); // Ejecuta el comando
    
    // Si execve falla, imprime error y termina el proceso hijo
    ft_putstr_fd2("error: cannot execute ", argv[0]);
    exit(1);
}

/* ============================
 *  FUNCIÓN PRINCIPAL (MAIN)
 * ============================ */

int main(int argc, char *argv[], char *env[]) {
    int i;
    int fd[2]; // Pipe: fd[0] = lectura, fd[1] = escritura
    int tmp_fd;
    (void)argc; // Evita advertencias de compilación por no usar `argc`

    tmp_fd = dup(STDIN_FILENO); // Duplica la entrada estándar original
    
    while (argv[i] && argv[i + 1]) { // Mientras haya argumentos por procesar
        argv = &argv[i + 1]; // Avanza a la siguiente secuencia de argumentos
        i = 0;
        
        // Busca el próximo `;` o `|` para separar comandos
        while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
            i++;

        /* ============================
         *  MANEJO DEL COMANDO CD
         * ============================ */
        if (strcmp(argv[0], "cd") == 0) { // Si el primer argumento es `cd`
            if (i != 2) // Verifica que tenga exactamente dos argumentos
                ft_putstr_fd2("error: cd: bad arguments", NULL);
            else if (chdir(argv[1]) != 0) // Intenta cambiar de directorio
                ft_putstr_fd2("error: cd: cannot change directory to ", argv[1]);
        }
        /* ============================
         *  EJECUCIÓN DE UN COMANDO SIN PIPES
         * ============================ */
        else if (i != 0 && (argv[i] == NULL || strcmp(argv[i], ";") == 0)) {
            // Si hay un comando y el separador es `NULL` (fin de argumentos) o `;`
            if (fork() == 0) // Crea un proceso hijo para ejecutar el comando
                ft_execute(argv, i, tmp_fd, env);
            else {
                close(tmp_fd); // Cierra el descriptor anterior
                while (waitpid(-1, NULL, WUNTRACED) != -1); // Espera a los procesos hijos
                tmp_fd = dup(STDIN_FILENO); // Restaura la entrada estándar
            }
        }
        /* ============================
         *  MANEJO DE PIPES (`|`)
         * ============================ */
        else if (i != 0 && strcmp(argv[i], "|") == 0) {
            // Si hay un comando y el separador es `|`
            if (argv[i + 1] == NULL) // Si `|` está al final sin comando después, error de sintaxis
                ft_putstr_fd2("error: syntax error", NULL);
            
            pipe(fd); // Crea un pipe (fd[0] = lectura, fd[1] = escritura)
            
            if (fork() == 0) { // Crea un proceso hijo para el primer comando
                dup2(fd[1], STDOUT_FILENO); // Redirige stdout al pipe
                close(fd[0]); // Cierra el extremo de lectura del pipe
                close(fd[1]); // Cierra el extremo de escritura (ya duplicado)
                ft_execute(argv, i, tmp_fd, env); // Ejecuta el comando
            } else {
                close(fd[1]); // Cierra el extremo de escritura en el padre
                close(tmp_fd); // Cierra el descriptor temporal
                tmp_fd = fd[0]; // La siguiente entrada estándar será la salida del pipe
            }
        }
    }
    close(tmp_fd); // Cierra el descriptor de entrada temporal
    return (0); // Finaliza el programa
}
