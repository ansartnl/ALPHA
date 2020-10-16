#ifndef AFTNPROTO_H
#define AFTNPROTO_H
#include <QtCore/QtGlobal>


#define AFTN_NAME_SIZE                          8
struct AftnAuthorizationRequest {
  qint8         aftnName[AFTN_NAME_SIZE];       //! AFTN client name
  quint8        pingTimeout;    //! PING timeout ( if 0 then 255)
};

#define AFTN_AUTHORIZATION_REPLY_CODE_SUCCESS   0
#define AFTN_AUTHORIZATION_REPLY_CODE_FAILED    1
struct AftnAuthorizationReply {
  qint8       code;               //! Reply code
};

#define AFTN_CHANNEL_ID_SIZE                    3
struct AftnAuthorizationInfo {
  qint8       channelId[AFTN_CHANNEL_ID_SIZE];      //! Identifier of channel
  qint8       aftnName[AFTN_NAME_SIZE];       //! AFTN client name
};

#define AFTN_PING_CODE_PING                     0
struct AftnPing {
  quint8 code;
};

#define AFTN_COMMAND
#define AFTN_COMMAND_PING                       0x00
#define AFTN_COMMAND_MAGIC                      0xFD
#define AFTN_COMMAND_CODE_INTERRUPT             0xFF
#define AFTN_COMMAND_CODE_CONTINUE              0xFE

#define AFTN_COMMAND_CODE_CONFIRM_MESSAGE       0x01

struct AftnCommand {
  quint8 magic;
  quint8 code;
};


#define AFTN_START_MESSAGE                      "ZCZC"
#define AFTN_END_MESSAGE                        "NNNN"
#endif
