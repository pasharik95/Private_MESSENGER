#! /bin/bash

# getting executable file
gcc -pthread -Iinclude -Isrc/ConversationWithClient/include -Isrc/Server/include -Isrc/WorkWithDB/include src/ConversationWithClient/conversation_with_client.c src/Server/server.c src/WorkWithDB/work_with_db.c src/main.c -lsqlite3 -o output/main

./output/main
