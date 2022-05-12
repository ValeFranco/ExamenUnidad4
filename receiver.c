#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

void* enviar(void *arg)
{

    struct mq_attr attr2;
    attr2.mq_flags = 0;
    attr2.mq_maxmsg = 10;
    attr2.mq_msgsize = 64;
    attr2.mq_curmsgs = 0;

    mqd_t mq2 = mq_open("/mq2", O_WRONLY| O_CREAT, 0644, &attr2);
    char str[64];

    while (1)
    {
        fgets(str, sizeof(str), stdin);
        if(str[strlen(str) - 1 ] == '\n') str[strlen(str) - 1 ] = 0;
        mq_send(mq2, str, strlen(str) + 1, 0);
        if (strncmp(str, "exit", strlen("exit")) == 0)
        {
            break;
        }
    }

    mq_close(mq2);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    mqd_t mq;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 64;
    attr.mq_curmsgs = 0;

    mq = mq_open("/mq0", O_RDONLY | O_CREAT, 0644, &attr);
    char buff[64];

    pthread_t threadID;
    pthread_create(&threadID,NULL,&enviar,NULL);

    while (1)
    {
        mq_receive(mq, buff, 64, NULL);
        printf("Message received: %s\n", buff);
        if( strncmp(buff, "exit", strlen("exit")) == 0){ //se comparan dos strings, si el string ingresado por el usurio es exit, se sale del ciclo 
            break; //strncmp devuelve cero si sus dos string a comparar son iguales
        }
        if(strncmp(buff, "Enviar Archivo", strlen("Enviar Archivo"))==0)
        {

        }
    }
 
    mq_close(mq); //cierra la cola
    mq_unlink("/mq0"); //remueve el queue de mensajes
    exit(EXIT_SUCCESS);
}