#ifndef MAIN_H
#define MAIN_H

#include <QtCore/QSettings>

#define SETTINGS_NETWORK_GROUP          "network"
#define SETTINGS_AFTNSERVER1            "aftn_server_1"
#define SETTINGS_AFTNSERVER2            "aftn_server_2"
#define SETTINGS_PORT                   "port"
#define SETTINGS_AFTN_NAME              "aftn_name"
#define SETTINGS_PING_TIMEOUT           "ping_timeout"
#define SETTINGS_LOG_FILE               "log_file"
#define SETTINGS_RAWDATA_FILE           "rawdata_file"

extern QSettings *networkSettings;

#endif //MAIN_H
