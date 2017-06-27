#include "conversation_with_client.h"

void error(char *msg)
{
    perror(msg);
    exit(1);
}

char *login(const char *buff, int sock, int *id_user)
{
    
    char *name = strtok(buff, DELIMITER);
    char *pass = strtok(NULL, DELIMITER);

    int id = checkUser(name, pass);
    int sock_now = getSockUser(name);

    if(id == NO_ID)
        return UNSUCCESS_LOGIN;

    if(sock_now != NO_SOCK)
        return ALREADY_IN_USE;

    updateSock(id, sock);
    updateStatus(id, ONLINE);

    *id_user = id;

    return SUCCESS_LOGIN;
}

char *regUser(const char *buff, int sock, int *id_user)
{
    char *name = strtok(buff, DELIMITER);
    char *pass = strtok(NULL, DELIMITER);
   
    int id = checkUserName(name);

    if(id != NO_ID)
        return UNSUCCESS_CREATION_ACC;

    addUser(name, pass, sock);
    
    *id_user = checkUser(name, pass);
    updateStatus(id_user, ONLINE);

    return SUCCESS_CREATION_ACC;  
}

char *sendTo(const char *buff)
{
    char *nameFrom = strtok(buff, DELIMITER);
    char *nameTo = strtok(NULL, DELIMITER);
    char *messege = strtok(NULL, DELIMITER);
    
    char statusFrom = getStatus(nameFrom);
    char statusTo= getStatus(nameTo);
    
    if(statusFrom == BUSY || statusFrom == OFFLINE)
        return RECEIVER_BUSY;

    if(statusTo == OFFLINE)
        return RECEIVER_OFFLINE;

    int sockTo = getSockUser(nameTo);

    size_t needed = snprintf(NULL, 0, "%c%s%s%s", RCV_MESSAGE, nameFrom, DELIMITER, messege);
    char *query  = malloc(needed + 1);
    sprintf(query, "%c%s%s%s", RCV_MESSAGE, nameFrom, DELIMITER, messege);
    send(sockTo, query, strlen(query) + 1, 0);

    return MESSAGE_SENT;  
}

void changeStatus(const char *buff)
{
    /* get values */
    char *name = strtok(buff, DELIMITER);
    char status = strtok(NULL, DELIMITER)[0];
    
    int id = getIdUser(name); 
    updateStatus(id, status);
    int *n = malloc(sizeof(int));
    char **users = getConnectedUsers(id, n);
    
    int i;
    for(i = 0; i < (*n); i++)
    {
        send(getSockUser(users[i]), getAll(users[i], SOME_STATUS), strlen(getAll(users[i], SOME_STATUS)), 0);
    }
}

char *sendToGroup(const char *buff)
{
    /* get values */
    char *nameFrom = strtok(buff, DELIMITER);
    int count = atoi(strtok(NULL, DELIMITER));
    char **usersNames = malloc(sizeof(char *) * count);
    int i;
    for(i = 0; i < count; i++)
    {
        usersNames[i] = strtok(NULL, DELIMITER);
    }
    char *messege = strtok(NULL, DELIMITER);
   
    char statusFrom = getStatus(nameFrom);
    
    if(statusFrom == BUSY || statusFrom == OFFLINE)
        return BROADCAST_MESSAGE_NOT_SENT;

    size_t needed_messege = snprintf(NULL, 0, "%c%s%s%s", RCV_MESSAGE, nameFrom, DELIMITER, messege);
    char *query  = malloc(needed_messege + 1);
    sprintf(query, "%c%s%s%s", RCV_MESSAGE, nameFrom, DELIMITER, messege);

    size_t needed_res = snprintf(NULL, 0, "%s%d", BROADCAST_MESSAGE_SENT, count);
    char *res = malloc(2 * count + needed_res);
    sprintf(res, "%s%d", BROADCAST_MESSAGE_SENT, count);
    
    for(i = 0; i < count; i++)
    {
         int sockTo = getSockUser(usersNames[i]);
         char statusTo= getStatus(usersNames[i]);
         if(NO_SOCK != sockTo && OFFLINE != statusTo)
         {
             send(sockTo, query, strlen(query) + 1, 0);
             strcpy((res + needed_res + 2 * i), SUCCESS_SEND);
         }
         else
             strcpy((res + needed_res + 2 * i), UNSUCCESS_SEND);
    } 
    return res;  
}

void *conversation(void *socketfd)
{
    char buffer[1024];
    char *res;
    int sock = *((int *)socketfd);
    int id_user = NO_ID;

    while(recv(sock, buffer, 1024, 0) > 0)
    {
        fprintf(stderr, "--%s--\n", buffer);
        switch(buffer[0])
        {
            case SEND_LOGIN_PASS    : send(sock, login(buffer + 1, sock, &id_user), 2, 0);
                       		      break;
            case SEND_USERNAME_PASS : send(sock, regUser(buffer + 1, sock, &id_user), 2, 0);
                                      break;
            case RCV_CONTACT_LIST   : send(sock, getAll(buffer + 1, SOME_STATUS), strlen(getAll(buffer + 1, SOME_STATUS)), 0);
                                      break;
            case MESSAGE            : send(sock, sendTo(buffer + 1), 2, 0);
                                      break;
            case STATUS_CHANGED     : changeStatus(buffer + 1);
                                      break;
            case BROADCAST_MESSAGE  : 
				       {
				           char *res = sendToGroup(buffer + 1);
				           send(sock, res, strlen(res), 0);
				           break;
				       }
        }
         memset(buffer, '\0', 1024);  
    }
    
    if(id_user != NO_ID)
    {
        updateSock(id_user, NO_SOCK);
        updateStatus(id_user, OFFLINE);
    }

    close(sock);
    pthread_exit(NULL);
}
