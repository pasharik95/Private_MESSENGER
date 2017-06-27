#ifndef COMMON_H
#define COMMON_H

#define IP_ADDRESS                      "127.0.0.1"               // server ip
#define PORT                            1111               // communication port

#define DELIMITER                       "#"
#define END_OF_STR                      '\n'
#define SEND_LOGIN_PASS                 '1'
#define UNSUCCESS_LOGIN                 "10"
#define SUCCESS_LOGIN                   "11"
#define ALREADY_IN_USE                  "12"
#define SEND_USERNAME_PASS              '2'
#define UNSUCCESS_CREATION_ACC          "20"
#define SUCCESS_CREATION_ACC            "21"
#define RCV_CONTACT_LIST                '3'
#define RCV_CONTACT_LIST_EMPTY          "3"
#define MESSAGE                         '4'
#define MESSAGE_SENT                    "41"
#define RECEIVER_BUSY                   "42"
#define RECEIVER_OFFLINE                "40"
#define RCV_MESSAGE                     '5'
#define STATUS_CHANGED                  '6'
#define BROADCAST_MESSAGE               '7'
#define BROADCAST_MESSAGE_NOT_SENT      "70"
#define BROADCAST_MESSAGE_SENT          "71"
#define ONLINE                          'o'
#define OFFLINE                         'f'
#define BUSY                            'b'
#define SOME_STATUS                     '-'
#define NO_ID                           -1
#define NO_SOCK                         -1
#define SUCCESS_SEND                    "#1"
#define UNSUCCESS_SEND                  "#0"

#endif // COMMON_H
