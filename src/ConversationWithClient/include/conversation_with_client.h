#ifndef CONVERSATION_WITH_CLIENT_H
#define CONVERSATION_WITH_CLIENT_H

#include <stdlib.h>
#include "all.h"
#include "common.h"
#include "work_with_db.h"

void error(char *msg);
char *login(const char *buff, int sock, int *id_user);
char *regUser(const char *buff, int sock, int *id_user);
char *sendTo(const char *buff);
void changeStatus(const char *buff);
char *sendToGroup(const char *buff);
void *conversation(void *socketfd);

#endif //CONVERSATION_WITH_CLIENT_H
