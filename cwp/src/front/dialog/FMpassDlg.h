#ifndef FM_PASSWORD_DLG_H
#define FM_PASSWORD_DLG_H

#include <QDialog>
#include <QButtonGroup>

//! Show FM values
class FMpassDlg : public QDialog
{
    Q_OBJECT

public:
    //! Constructor
    /*!
      \param pParent a parent widget
      \param f a widget's flags
    */
    FMpassDlg(QWidget* pParent = 0, Qt::WindowFlags f = 0);
    //! Destructor
    virtual ~FMpassDlg();

public:
    static void getFMvalues(QString& val);

public slots:
    virtual void accept();

private:
    void Init();
    void Translate();
    static int GetFMval();
    void SetFMval(int val);
    QString GetPass();
    void SetPass(QString val);
    QString CryptPass(QString pass);

private:
    QButtonGroup* mButGroup;
};

#endif // FM_PASSWORD_DLG_H
