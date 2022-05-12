#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

void* recibir(void *arg)
{
   mqd_t mq2 = mq_open("/mq2",O_RDONLY);

    char buff[200];

    while (1)
    {
        mq_receive(mq2, buff, 200, NULL);
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
    char str[200];

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

        if( strncmp(str, "EnviarMensajeNormal", strlen("EnviarMensajeNormal")) == 0){  //lee txt y lo envia
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
        // else if(strncmp(str, "EnviarMensajeInvertido", strlen("EnviarMensajeInvertido")) == 0)
        // {
        //     FILE *fPointer;
        //     fPointer = fopen("MiMensaje.txt", "r");

        //     char singleLine[200];

        //     char multipleLines[200][200];

        //     int counter = 0;
        //     int tot = 0;

        //      while (fgets(multipleLines[counter], 200, fPointer))
        //     {
        //         multipleLines[counter][strlen(multipleLines[counter])-1]= '\n';
        //         counter++;
        //     }
            
        //     tot = counter;

        //     for (counter = 0; counter < tot; ++counter) //falta leerlo alrevez, aca lo lee normal si funcionara
        //     {
        //         mq_send(mq, multipleLines[counter], strlen(str) + 1, 0);
        //     }
            

        // }
        

    }

    mq_close(mq);
    exit(EXIT_FAILURE);
}
