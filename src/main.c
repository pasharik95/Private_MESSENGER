#include "all.h"

int main(int argc, char *argv[])
{
    initDB(); // create DB
    startServer(); // start server
    setOfflineAll(); // if server stop - set all user's status - offline

    return 0;
}
