#ifndef MULTILINEINPUTDIALOG_H
#define MULTILINEINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class MultiLineInputDialog;
}

class MultiLineInputDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MultiLineInputDialog(QWidget *parent = 0);
    ~MultiLineInputDialog();

    QString getText() const;
    void setText(const QString &str) const;
    
private slots:
    void accept();

private:
    Ui::MultiLineInputDialog *ui;
    QString mData;
};

#endif // MULTILINEINPUTDIALOG_H
