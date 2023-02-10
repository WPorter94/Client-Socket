#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(int argc,char *argv[]){
    struct sockaddr_in server;
    char *buffer = (char *)malloc(200);
    char cs[20] = "cs";
    int csnums = 230;
    char space[] = " ";
    char hello[] = " HELLO ";
    char end[] = "\n";
    char *address = (char *)malloc(200);

    sprintf(cs,"cs%d",csnums);
    strcpy(address, argv[1]);
    strcpy(buffer, argv[1]);

    //creates socket
    int soc = socket(AF_INET,SOCK_STREAM,0);
    if(soc < 0){
        printf("Error, socket failed.\n");
        return 0;
    }
    printf("Socket success.\n");

        //creates server profile
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    int check = inet_pton(AF_INET, argv[3], &server.sin_addr);
    memset(&(server.sin_zero), '\0', 8);

    if(check <= 0){
        printf("Error, inet_pton failed.\n");
        return 0;
    }
    printf("inet_pton success.\n");

    //connects to server
    int con = connect(soc, (struct sockaddr *)&server, sizeof(server));
    if(con < 0){
        printf("Error, connect failed.\n");
        return 0;
    }
    printf("Connect success.\n");
    bzero(buffer, sizeof(buffer));
    //strcat(buffer, cs);
    //strcat(buffer, hello);
    //strcat(buffer, address);
    //strcat(buffer,"\n");
    sprintf(buffer,"%s%s%s\n",cs,hello,address);
    int sent = send(soc, buffer, strlen(buffer),0);
    if(sent < 0)
    {
        printf("Error, send failed.\n");
        return 0;
    }
    printf("Send Success.\n");
    printf("%s",buffer);
    bzero(buffer, sizeof(buffer));

    int num1 = 0;
    int num2 = 0;
    int num3 = 0;
    int num4 = 0;
    int num5 = 0;
    int temp = 0;
    //int numArr[5] = {};
    char* op = (char *)malloc(100);
    char* status = (char *)malloc(100);
    char* min = "min";
    char* max = "max";
    char* med = "median";
    char* flag = "STATUS\n";
    //char delim[2] = " ";
    char *recv_mess = (char *)malloc(200);

    while(1){
        int recieve = recv(soc, buffer, 200, 0);
        if(recieve < 0){
            printf("Error, recieve failed.\n");
            return 0;
        }       
        printf("Recieved succesfully: ");
        buffer[recieve] = '\0';
        printf("%s\n",buffer);
        status = strtok(buffer, " ");
        status = strtok(NULL, " ");
        
        if(strcmp(flag,status)){
            op = strtok(NULL, " ");
            num1 = atoi(strtok(NULL, " "));
            num2 = atoi(strtok(NULL, " "));
            num3 = atoi(strtok(NULL, " "));
            num4 = atoi(strtok(NULL, " "));
            num5 = atoi(strtok(NULL, " "));
            if(strcmp(op, min) == 0){
                if(num2 < num1){ num1 = num2;}
                if(num3 < num1){ num1 = num3;}
                if(num4 < num1){ num1 = num4;}
                if(num5 < num1){ num1 = num5;}
            }else if(strcmp(op, max) == 0){
                if(num2 > num1){ num1 = num2;}
                if(num3 > num1){ num1 = num3;}
                if(num4 > num1){ num1 = num4;}
                if(num5 > num1){ num1 = num5;}
            }else if(strcmp(op, med) == 0){
                int numArr[5] = {num1,num2,num3,num4,num5};
                for(int i = 0; i < 5; ++i){
                    for(int j = 0; j < 4; j++){
                        if(numArr[j]>numArr[j+1]){
                        temp = numArr[j];
                        numArr[j] = numArr[j+1];
                        numArr[j+1] = temp;
                        } 
                    }
                }
                   num1 = numArr[2];
            }
        }else{
            printf("flag recieved: %s", status);
            return 0;
        }
        bzero(buffer, sizeof(buffer));
        sprintf(buffer, "%s %d\n",cs, num1 );
        sent = send(soc, buffer, strlen(buffer),0);
        if(sent < 0)
        {
            printf("Error, send failed.\n");
            return 0;
        }
        printf("Sent successfully: ");
        printf("%s\n",buffer);
        bzero(buffer, sizeof(buffer));

    }
    return 0;
}