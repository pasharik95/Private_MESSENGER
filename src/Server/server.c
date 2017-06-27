#include "server.h"

void startServer()
{
    int welcomeSocket, newSocket;
    int no = 1;
    pthread_t thread_id; // thread for calculation
    pthread_attr_t attr; // attributes for thread
   
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    int pthread_return_value;    

    pthread_attr_init(&attr); // init attributes

    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    
    /* setsockopt - function for setting socket options:
     * setting SO_REUSEADDR for reusing port */
    if (-1 == setsockopt(welcomeSocket, SOL_SOCKET,SO_REUSEADDR, &no, sizeof(int)))
    {
        error("setsockopt()");
    }    

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(PORT);
    /* Set IP address */
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    /*---- Bind the address struct to the socket ----*/
    if(0 > bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)))
        error("bind()");

    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket, 5)==0)
      printf("Listening\n");
    else
      printf("Error\n");
    
    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof serverStorage;

    while(0 < (newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size)))
        pthread_create(&thread_id, &attr, &conversation, (void *)&newSocket); // create new thread for each new client
}

void stopServer()
{
    exit(0);
}
