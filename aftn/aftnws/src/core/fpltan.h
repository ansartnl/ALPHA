#ifndef FPL_TAN_H
#define FPL_TAN_H

#include "fpl.h"

class FPLfromTAN : public FPL
{
public:
    static const QString TBL_FPL_CHANGES;
    static const QString TBL_FPL_CHANGES_OPER;
    static const QString TBL_FPL_CHANGES_OPER_TIME;
    static const QString TBL_FPL_CHANGES_MESSAGE_TEXT;

    /** Constructor */
    FPLfromTAN(const QString& table = TBL_FPL_CHANGES, const QString& tableID = TBL_FPL_ID);

    /** Destructor */
    virtual ~FPLfromTAN();

  public:

    /** Fill query for INSERT request */
    virtual void prepareInsert(QSqlQuery &query);

    /** Fill query for UPDATE request */
    virtual void prepareUpdate(QSqlQuery &query);

    /** Fill query for SELECT request */
    virtual void prepareSelect(QSqlQuery &query);

    /** Update information after SELECT request */
    virtual void updateInfo(QSqlQuery &query);
    virtual void updateInfo(const QSqlRecord& record);

    /** Set parameters to DB request */
    virtual void updateQuery(QSqlQuery &query);

    /** Fill query for SELECT ALL request */
    static void prepareSelectAll(QSqlQuery &query, const QString& sTable = TBL_FPL_CHANGES, const QString& sWhere = QString::null);

    /** Clone object (full copy without Id) */
    FPLfromTAN clone(const QString& sTable = QString::null) const;

  public:

    /** Type operation */
    const QString& oper() const;
    bool setOper(const QString& sOper);

    /** Time operation */
    const QDateTime& operTime() const;
    bool setOperTime(const QDateTime& dtOperTime);

    /** Message text */
    const QString& messageText() const;
    bool setMessageText(const QString& sMessageText);

  private:
    QString     mOper;
    QDateTime   mOperTime;
    QString     mMessageText;
};

class FPLfromFDP : public FPL
{
public:
    static const QString TBL_FPL_CHANGES;
    static const QString TBL_FPL_CHANGES_OPER;
    static const QString TBL_FPL_CHANGES_OPER_TIME;
    static const QString TBL_FPL_CHANGES_MESSAGE_TEXT;
    static const QString TBL_FPL_CHANGES_STATUS;
    static const QString TBL_FPL_CHANGES_POD_IN;
    static const QString TBL_FPL_CHANGES_T_IN;
    static const QString TBL_FPL_CHANGES_H_IN;
    static const QString TBL_FPL_CHANGES_POD_OUT;
    static const QString TBL_FPL_CHANGES_T_OUT;
    static const QString TBL_FPL_CHANGES_H_OUT;
    static const QString TBL_FPL_CHANGES_FPL_ID;

    static const QString TBL_MSG_FROM_FDP;

    /** Constructor */
    FPLfromFDP(const QString& table = TBL_FPL_CHANGES, const QString& tableID = TBL_FPL_ID);
    FPLfromFDP(const FPL& fpl);

    /** Destructor */
    virtual ~FPLfromFDP();

  public:

    /** Fill query for INSERT request */
    virtual void prepareInsert(QSqlQuery &query);

    /** Fill query for UPDATE request */
    virtual void prepareUpdate(QSqlQuery &query);

    /** Fill query for SELECT request */
    virtual void prepareSelect(QSqlQuery &query);

    /** Update information after SELECT request */
    virtual void updateInfo(QSqlQuery &query);
    virtual void updateInfo(const QSqlRecord& record);

    /** Set parameters to DB request */
    virtual void updateQuery(QSqlQuery &query);

    /** Fill query for SELECT ALL request */
    static void prepareSelectAll(QSqlQuery &query, const QString& sTable = TBL_FPL_CHANGES, const QString& sWhere = QString::null);

    /** Clone object (full copy without Id) */
    FPLfromFDP clone(const QString& sTable = QString::null) const;

  public:

    /** Type operation */
    const QString& oper() const;
    bool setOper(const QString& sOper);

    /** Time operation */
    const QDateTime& operTime() const;
    bool setOperTime(const QDateTime& dtOperTime);

    /** Message text */
    const QString& messageText() const;
    bool setMessageText(const QString& sMessageText);

    /** The current status of the plan: 0-free, 1-activated, 2-assumed, 3-transferred */
    int status() const;
    void setStatus(int st);

    //! Entry point
    //{@
    const QString& podIn() const;
    bool setPodIn(const QString& podIn);

    const QDateTime& tIn() const;
    bool setTIn(const QDateTime& dt);

    const QString& hIn() const;
    bool setHIn(const QString& hIn);
    //@}

    //! Exit point
    //{@
    const QString& podOut() const;
    bool setPodOut(const QString& podOut);

    const QDateTime& tOut() const;
    bool setTOut(const QDateTime& dt);

    const QString& hOut() const;
    bool setHOut(const QString& hOut);
    //@}

    const QString& fpl_ID() const;
    bool setFPL_ID(const QString& fpl_ID);

  private:
    QString     mOper;
    QDateTime   mOperTime;
    QString     mMessageText;
    int         mStatus;
    QString     mPodIn;
    QDateTime   mTin;
    QString     mHin;
    QString     mPodOut;
    QDateTime   mTout;
    QString     mHout;
    QString     mFPL_ID;
};

#endif // FPL_TAN_H
