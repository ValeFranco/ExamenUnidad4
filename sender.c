#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

void* recibir(void *arg)
{
   mqd_t mq2 = mq_open("/mq2",O_RDONLY);

    char buff[64];

    while (1)
    {
        mq_receive(mq2, buff, 64, NULL);
        printf("Message received: %s\n", buff);
        if (strncmp(buff, "exit", strlen("exit")) == 0) {
            break;
        }

    }
    mq_close(mq2);
    mq_unlink("/mq2");
    exit(EXIT_SUCCESS);
}
int main(int argc, char *argv[])
{
    mqd_t mq = mq_open("/mq0", O_WRONLY);
    char str[64];

    pthread_t threadID2;
    pthread_create(&threadID2,NULL,&recibir,NULL);

    while (1)
    {
        fgets(str, sizeof(str), stdin);
        if(str[strlen(str) - 1 ] == '\n') str[strlen(str) - 1 ] = 0;
        mq_send(mq, str, strlen(str) + 1, 0);
        if (strncmp(str, "exit", strlen("exit")) == 0)
        {
            break;
        }

        if( strncmp(str, "EnviarMensaje", strlen("EnviarMensaje")) == 0){  //lee txt y lo envia
            //falta agregar si leer arriba hacia abajo o alrevez
            FILE *fPointer;
            fPointer = fopen("MiMensaje.txt", "r");
            char singleLine[200];
            while (!feof(fPointer))
            {
                fgets(singleLine, 200, fPointer);
                mq_send(mq, singleLine, strlen(str) + 1, 0);

            }
            fclose(fPointer);
        }

    }

    mq_close(mq);
    exit(EXIT_FAILURE);
}
