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
    attr2.mq_msgsize = 200;
    attr2.mq_curmsgs = 0;

    mqd_t mq2 = mq_open("/mq2", O_WRONLY| O_CREAT, 0644, &attr2);
    char str[200];

    while (1)
    {
        fgets(str, sizeof(str), stdin);
        if(str[strlen(str) - 1 ] == '\n') str[strlen(str) - 1 ] = 0;
        mq_send(mq2, str, strlen(str) + 1, 0);
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
                mq_send(mq2, singleLine, strlen(str) + 1, 0);

            }
            fclose(fPointer);
        }
        // else if(strncmp(str, "EnviarMensajeInvertido", strlen("EnviarMensajeInvertido")) == 0)
        // {
        //     FILE *fPointer;
        //     fPointer = fopen("MiMensaje.txt", "r");

        //     char singleLine[200];

        //     char multipleLines[200][200];//

        //     int counter = 0;
        //     int tot = 0;

        //      while (fgets(multipleLines[counter], 200, fPointer))
        //     {
        //         multipleLines[counter][strlen(multipleLines[counter])-1]= '\n';
        //         counter++;
        //     }

        //     tot = counter;

        //     for (counter = 0; counter < tot; ++counter)
        //     {
        //         mq_send(mq2, multipleLines[counter], strlen(str) + 1, 0);
        //     }
            

        // }

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
    attr.mq_msgsize = 200;
    attr.mq_curmsgs = 0;

    mq = mq_open("/mq0", O_RDONLY | O_CREAT, 0644, &attr);
    char buff[200];

    pthread_t threadID;
    pthread_create(&threadID,NULL,&enviar,NULL);

    while (1)
    {
        mq_receive(mq, buff, 200, NULL);
        printf("Message received: %s\n", buff);
        if( strncmp(buff, "exit", strlen("exit")) == 0){ //se comparan dos strings, si el string ingresado por el usurio es exit, se sale del ciclo 
            break; //strncmp devuelve cero si sus dos string a comparar son iguales
        }

        

    }
 
    mq_close(mq);
    mq_unlink("/mq0"); //remueve el queue de mensajes
    exit(EXIT_SUCCESS);
}