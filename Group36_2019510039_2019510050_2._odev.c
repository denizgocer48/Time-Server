#include<stdio.h>
#include<string.h>    // for strlen
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
#include <time.h>
 
 #define PORT_NUMBER 60000

int main(int argc, char *argv[])
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char input[5000]; // User input

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    //Address Family - AF_INET (this is IP version 4)
    //Type - SOCK_STREAM (this means connection oriented TCP protocol)
    //Protocol - 0 (Specifies a particular protocol to be used with the socket...
    //Specifying a protocol of 0 causes socket() to use an unspecified default protocol) 
    //[ or IPPROTO_IP This is IP protocol]
    if (socket_desc == -1)
    {
        puts("Could not create socket");
        return 1;
    }
     
    server.sin_family = AF_INET;  //IPv4 Internet protocols
    server.sin_addr.s_addr = INADDR_ANY; //IPv4 local host address
    server.sin_port = htons(PORT_NUMBER); // server will listen to 600000 port
    
    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Binding failed");
        return 1;
    }
    puts("Socket is binded");
     
    listen(socket_desc, 3);
     
    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    
    c = sizeof(struct sockaddr_in);
    new_socket= accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (new_socket<0)
    {
        puts("Accept failed");
        return 1;
    }
     
    puts("Connection accepted");
    
    int n;

    time_t rawtime; // time_t is a data type to represent time
    struct tm *info; // time info
    char b[80]; 

    time(&rawtime); // get current time
    info = localtime(&rawtime); // convert to local time

    
    send(new_socket, "input: ", strlen("input: "), 0);

    // Reply to the client
    while ((n = recv(new_socket, input, sizeof(input), 0)) > 0) // Continues to receive input from the user until the user enters the "CLOSE_SERVER" input
    {
        int m = 0; // counts the length of the user's input
        
        for (int i = 0; input[i] != '\r'; i++)
        {
            if(input[i] >= 'a' && input[i] <= 'z') 
            {
                input[i] = input[i] - 32;
            }
            m++;
        }

        char input2[m + 1]; // transfers user input to an array of input size to compare with commands
        for (int i = 0; input[i]!='\0'; i++)
        {
            input2[i] = input[i]; 
        }
        input2[m] = '\0';

        if (strcmp("GET_TIME", input2) == 0) // compare user input with commands
        {
            strftime(b, sizeof(b), "%X\n", info); // It serves to keep the information requested by the user
            send(new_socket, b, strlen(b),0); // returns the requested information to the user
        }
        else if (strcmp("GET_DATE", input2) == 0)
        {
            strftime(b, sizeof(b), "%x\n", info);
            send(new_socket, b, strlen(b), 0);
        }
        else if (strcmp("GET_TIME_DATE", input2) == 0)
        {
            strftime(b,sizeof(b),"%X, ", info);
            send(new_socket, b, strlen(b),0);
            strftime(b,sizeof(b),"%x\n", info);
            send(new_socket, b, strlen(b),0);
        }   
        else if (strcmp("GET_TIME_ZONE", input2) == 0)
        {
            strftime(b,sizeof(b),"%Z\n", info);
            send(new_socket, b, strlen(b),0);
        }
        else if (strcmp("GET_DAY_OF_WEEK", input2) == 0)
        {
            strftime(b,sizeof(b),"%A\n", info);
            send(new_socket, b, strlen(b),0);
        }
        else if (strcmp("CLOSE_SERVER", input2) == 0) // to close the server
        {
            send(new_socket, "GOOD BYE\n", strlen("GOOD BYE\n"), 0);
            break;
        }
        else
        {
            send(new_socket, "INCORRET REQUEST\n", strlen("INCORRET REQUEST\n"), 0);
        }
        send(new_socket, "input: ", strlen("input: "), 0);
    }

     close(socket_desc);
     close(new_socket); 

    return 0;
}