#ifndef NOTAM_H
#define NOTAM_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>

class QSqlRecord;
class QSqlQuery;

/// Class-wraper for parsed NOTAMs
class Notam
{
  public:

    /** Constructor */
    Notam();
    Notam(int iId);
    Notam(const QSqlRecord& record);

    /** Destructor */
    ~Notam();

  private:

    int mId;
    QString mSeries;
    int mSnumber;
    int mYear;
    QString mNature;
    int mReplSnumber;
    int mReplYear;
    QString mFir;
    QString mSubject;
    QString mCondition;
    QString mTraffic;
    QString mPurpose;
    QString mScope;
    int mQLowerLimit;
    int mQUpperLimit;
    QString mLocation;
    int mRadius;
    QString mAerodrom;
    QDateTime mValidFrom;
    QDateTime mValidUntil;
    QString mValidUntilT;
    QString mSchedule;
    QString mDescription;
    QString mLowerLimit;
    QString mUpperLimit;

  public:

    inline int id() const
    {
      return mId;
    }
    inline const QString& series() const
    {
      return mSeries;
    }
    inline int serialNumber() const
    {
      return mSnumber;
    }
    inline int serialYear() const
    {
      return mYear;
    }
    inline const QString& nature() const
    {
      return mNature;
    }
    inline int replaceSerialNumber() const
    {
      return mReplSnumber;
    }
    inline int replaceSerialYear() const
    {
      return mReplYear;
    }
    inline const QString& FIR() const
    {
      return mFir;
    }
    inline const QString& subject() const
    {
      return mSubject;
    }
    inline const QString& condition() const
    {
      return mCondition;
    }
    inline const QString& traffic() const
    {
      return mTraffic;
    }
    inline const QString& purpose() const
    {
      return mPurpose;
    }
    inline const QString& scope() const
    {
      return mScope;
    }
    inline int qLowerLimit() const
    {
      return mQLowerLimit;
    }
    inline int qUpperLimit() const
    {
      return mQUpperLimit;
    }
    inline const QString& location() const
    {
      return mLocation;
    }
    inline int radius() const
    {
      return mRadius;
    }
    inline const QString& aerodrome() const
    {
      return mAerodrom;
    }
    inline const QDateTime& validFrom() const
    {
      return mValidFrom;
    }
    inline const QDateTime& validUntil() const
    {
      return mValidUntil;
    }
    inline const QString& validUntilT() const
    {
      return mValidUntilT;
    }
    inline const QString& schedule() const
    {
      return mSchedule;
    }
    inline const QString& description() const
    {
      return mDescription;
    }
    inline const QString& lowerLimit() const
    {
      return mLowerLimit;
    }
    inline const QString& upperLimit() const
    {
      return mUpperLimit;
    }

  public:

    /** Fill query for SELECT request */
    virtual void prepareSelect(QSqlQuery& query);

    /** Update information after SELECT request */
    virtual void updateInfo(QSqlQuery& query);
    virtual void updateInfo(const QSqlRecord& query);

    /** Fill query for DELETE request */
    virtual void prepareDelete(QSqlQuery& query);

    /** Set parameters to DB request */
    virtual void updateQuery(QSqlQuery& query);

    /** Fill query for SELECT ALL request */
    static void prepareSelectAll(QSqlQuery& query);

    /** Clone object (full copy without Id) */
    Notam clone() const;
};

#endif // NOTAM_H
