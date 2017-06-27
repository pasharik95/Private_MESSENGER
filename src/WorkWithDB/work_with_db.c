#include "work_with_db.h"

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    for(i = 0; i < argc; i++)
    {
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void initDB()
{
    char *zErrMsg = 0;
    FILE *fp;
    // open file with DB
    if((fp = fopen("users.db", "r")) != NULL)
        return(0);

    // Create file users.db    
    fp = fopen("users.db", "ab+");

    /* Open database */
    rc = sqlite3_open("users.db", &db);
   
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

    /* Create SQL statement */
    sql = "CREATE TABLE USERS("  \
          "ID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL," \
          "NAME TEXT NOT NULL," \
          "PASSWORD STRING NOT NULL," \
          "STATUS CHAR NOT NULL," \
          "SOCK INTEGER);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
}

int checkUser(char *name, char *password)
{
    sqlite3_stmt *id;
    char *zErrMsg = 0;
    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }
    size_t needed = snprintf(NULL, 0, "SELECT ID FROM USERS\
    WHERE NAME = \"%s\" AND PASSWORD = \"%s\";", name, password);
    char *query  = malloc(needed + 1);
    sprintf(query, "SELECT ID FROM USERS WHERE NAME = \"%s\" AND PASSWORD = \"%s\";", name, password);
    rc = sqlite3_prepare_v2(db, query, -1, &id, 0);

    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data : %s\n", sqlite3_errmsg(db));
        return 0;
    }

    rc = sqlite3_step(id);
    int id_value = sqlite3_column_int(id, 0);
    if(id_value == NULL)
        id_value = -1;

    sqlite3_finalize(id);
    sqlite3_close(db);

    return id_value;
}

int checkUserName(char *name)
{
     sqlite3_stmt *id;
    char *zErrMsg = 0;

    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }
    
    size_t needed = snprintf(NULL, 0, "SELECT ID FROM USERS WHERE NAME = \"%s\";", name);
    char *query  = malloc(needed + 1);
    sprintf(query, "SELECT ID FROM USERS WHERE NAME = \"%s\";", name);
    rc = sqlite3_prepare_v2(db, query, -1, &id, 0);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data : %s\n", sqlite3_errmsg(db));
        return 0;
    }

    rc = sqlite3_step(id);

    int id_value = sqlite3_column_int(id, 0);
    if(id_value == NULL)
        id_value = -1;
    sqlite3_finalize(id);
    sqlite3_close(db);

    return id_value;
}

int getIdUser(char *name)
{
    sqlite3_stmt *id;
    char *zErrMsg = 0;

    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }
    
    size_t needed = snprintf(NULL, 0, "SELECT ID FROM USERS WHERE NAME = \"%s\";", name);
    char *query  = malloc(needed + 1);
    sprintf(query, "SELECT ID FROM USERS WHERE NAME = \"%s\";", name);
    rc = sqlite3_prepare_v2(db, query, -1, &id, 0);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data : %s\n", sqlite3_errmsg(db));
        return 0;
    }

    rc = sqlite3_step(id);
    int id_value = sqlite3_column_int(id, 0);
    
    sqlite3_finalize(id);
    sqlite3_close(db);

    return id_value;
}

char getStatus(char *name)
{
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;

    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }
    
    size_t needed = snprintf(NULL, 0, "SELECT STATUS FROM USERS WHERE NAME = \"%s\";", name);
    char *query  = malloc(needed + 1);
    sprintf(query, "SELECT STATUS FROM USERS WHERE NAME = \"%s\";", name);
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data : %s\n", sqlite3_errmsg(db));
        return 0;
    }

    rc = sqlite3_step(stmt);
    char status = sqlite3_column_text(stmt, 0)[0];
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return status;
}

void addUser(char *name, char *password, int sock)
{
    char *zErrMsg = 0;

    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if(rc)
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    size_t needed = snprintf(NULL, 0, "INSERT INTO USERS (NAME, PASSWORD, STATUS, SOCK) VALUES(\"%s\", \"%s\", \'%c\', %d);", name, password, 'o', sock);
    char *query  = malloc(needed + 1);
    sprintf(query, "INSERT INTO USERS (NAME, PASSWORD, STATUS, SOCK) VALUES(\"%s\", \"%s\", \'%c\', %d);", name, password, 'o', sock);
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }
    else
    {
       fprintf(stdout, "Records created successfully\n");
    }
    sqlite3_close(db);
    free(query);
}

void updateSock(int id, int sock)
{
    char *zErrMsg = 0;
    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    size_t needed = snprintf(NULL, 0, "UPDATE USERS SET SOCK = \'%d\' WHERE ID = %d;", sock, id);
    char *query  = malloc(needed + 1);
    sprintf(query, "UPDATE USERS SET SOCK = \'%d\' WHERE ID = %d;", sock, id);

    /* Execute SQL statement */
    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }
    else
    {
       fprintf(stdout, "Records updated successfully\n");
    }
    sqlite3_close(db);
    free(query);
}

void updateStatus(int id, char status)
{
    char *zErrMsg = 0;
    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    size_t needed = snprintf(NULL, 0, "UPDATE USERS SET STATUS = \'%c\' WHERE ID = %d;", status, id);
    char *query  = malloc(needed + 1);
    sprintf(query, "UPDATE USERS SET STATUS = \'%c\' WHERE ID = %d;", status, id);

    /* Execute SQL statement */
    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }
    else
    {
       fprintf(stdout, "Records updated successfully\n");
    }
    sqlite3_close(db);
    free(query);
}

void setOfflineAll()
{
    char *zErrMsg = 0;
    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    size_t needed = snprintf(NULL, 0, "UPDATE USERS SET STATUS = \'%c\', SOCK = %d;", 'f', -1);
    char *query  = malloc(needed + 1);
    sprintf(query, "UPDATE USERS SET STATUS = \'%c\', SOCK = %d;", 'f', -1);

    /* Execute SQL statement */
    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK )
    {
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
    free(query);
}


char *getAll(char *name_user,char status_user)
{
    sqlite3_stmt *stmt;
    sqlite3_stmt *stmt_new;
    char *zErrMsg = 0;

    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }
    /* Create SQL statement */
    size_t needed;
    char *query;
    
    if('-' != status_user)
    {
        needed = snprintf(NULL, 0, "SELECT NAME, STATUS FROM USERS WHERE NAME <> \"%s\" AND STATUS = \'%c\';", name_user, status_user);
        query  = malloc(needed + 1);
        sprintf(query, "SELECT NAME, STATUS FROM USERS WHERE NAME <> \"%s\" AND STATUS = \'%c\';", name_user, status_user);
    }
    else
    {
        needed = snprintf(NULL, 0, "SELECT NAME, STATUS FROM USERS WHERE NAME <> \"%s\";", name_user);
        query  = malloc(needed + 1);
        sprintf(query, "SELECT NAME, STATUS FROM USERS WHERE NAME <> \"%s\";", name_user);
    }
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data : %s\n", sqlite3_errmsg(db));

        return 0;
    }
    /* allocate memory for decsription and venue */
    char *name = (char *)malloc(100);
    char status;
    stmt_new = stmt;
    char ** users;
    char *res;
    int n = 0;
    
    /* loop reading each row until step returns anything other than SQLITE_ROW */
    do
    {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        { 
            n++;
	}
    }while (rc == SQLITE_ROW);
    
    users = (char **)malloc(sizeof(char *) * n);

    /* loop reading each row until step returns anything other than SQLITE_ROW */
    int i = 0;
    n = 0;
    do
    {
        rc = sqlite3_step(stmt_new);
        if (rc == SQLITE_ROW)
        { 
            strcpy(name, (char *)sqlite3_column_text(stmt, 0));
            status = sqlite3_column_text(stmt, 1)[0];

            size_t needed = snprintf(NULL, 0, "%s#%c\n", name, status);
            n += needed;

            users[i]  = malloc(needed);

            sprintf(users[i], "%s#%c\n", name, status);
            i++;
	}
    }while (rc == SQLITE_ROW);
    fprintf(stderr, "))%d((", i);
    /* finish off */
    sqlite3_close(db);
    free(name);

    if(0 == i)
        return RCV_CONTACT_LIST_EMPTY;

    res = (char *)malloc(n + 1);
    memset(res, 0, n + 1);
    int j;
    for(j = 0; j < i; j++)
        strcat(res + 1, users[j]);
    res[0] = RCV_CONTACT_LIST;
    return res;
}


int getSockUser(char *name)
{
    sqlite3_stmt *sock;
    char *zErrMsg = 0;

    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }
    
    size_t needed = snprintf(NULL, 0, "SELECT SOCK FROM USERS WHERE NAME = \"%s\";", name);
    char *query  = malloc(needed + 1);
    sprintf(query, "SELECT SOCK FROM USERS WHERE NAME = \"%s\";", name);
    rc = sqlite3_prepare_v2(db, query, -1, &sock, 0);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data : %s\n", sqlite3_errmsg(db));
        return 0;
    }

    rc = sqlite3_step(sock);
    int sock_number = sqlite3_column_int(sock, 0);
    
    sqlite3_finalize(sock);
    sqlite3_close(db);

    return sock_number;
}

char **getConnectedUsers(int id, int *n)
{
    sqlite3_stmt *stmt_new;
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;

    /* Open database */
    rc = sqlite3_open("users.db", &db);
    if( rc )
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    }
    else
    {
       fprintf(stderr, "Opened database successfully\n");
    }
    /* Create SQL statement */
    size_t needed = snprintf(NULL, 0, "SELECT NAME FROM USERS WHERE ID <> \"%d\" AND SOCK <> -1;", id);;
    char *query = malloc(needed + 1);
    sprintf(query, "SELECT NAME FROM USERS WHERE ID <> \"%d\" AND SOCK <> -1;", id);
   
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data : %s\n", sqlite3_errmsg(db));

        return 0;
    }
    /* allocate memory for decsription and venue */
    char *name = (char *)malloc(100);
    stmt_new = stmt;
    char ** users;
    *n = 0;
    
    /* loop reading each row until step returns anything other than SQLITE_ROW */
    do
    {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        { 
            (*n)++;
	}
    }while (rc == SQLITE_ROW);
    
    users = (char **)malloc(sizeof(char *) * (*n));

    /* loop reading each row until step returns anything other than SQLITE_ROW */
    int i = 0;
    do
    {
        rc = sqlite3_step(stmt_new);
        if (rc == SQLITE_ROW)
        { 
            strcpy(name, (char *)sqlite3_column_text(stmt, 0));

            size_t needed = snprintf(NULL, 0, "%s", name);

            users[i]  = malloc(needed);

            sprintf(users[i], "%s", name);
            i++;
	}
    }while (rc == SQLITE_ROW);

    /* finish off */
    sqlite3_close(db);
    free(name);
    return users;
}
