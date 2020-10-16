#ifndef PULSAR_FDP_SETTINGS_H
#define PULSAR_FDP_SETTINGS_H

namespace PulsarSettings
{
    const char fdpName[] = "fdp_name";

    const char autoAnswerOnError[] = "auto_answer_on_error";

    const char cksIsConnected[] = "cks_is_connected";

    const char telegramNextNumNet[] = "telegram_next_num_net";
    const char telegramNextNumNetDate[] = "telegram_next_num_net_date";
    const char telegramNextNumFile[] = "telegram_next_num_file";
    const char telegramNextNumFileDate[] = "telegram_next_num_file_date";

    enum AutoSent
    {
        NONE  = 0,
        START = 1,
        STOP  = 2
    };
    const char autoSent[] = "auto_sent";

    const char channelId[] = "channel_id";
}

#endif // PULSAR_FDP_SETTINGS_H
