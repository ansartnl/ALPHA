#ifndef AFTNDAO_H
#define AFTNDAO_H

#include <QtSql/QSqlError>

class FPL;
class RPL;
class Notam;

struct AFTNDao
{
    static QSqlError loadFPL(FPL& Fpl);
    static QSqlError saveFPL(FPL& Fpl);
    static QSqlError findFPL(FPL& Fpl);
    static QSqlError loadRPL(RPL& Rpl);
    static QSqlError saveRPL(RPL& Rpl);
    static QSqlError loadNOTAM(Notam& Notam);
};
#endif //AFTNDAO_H
