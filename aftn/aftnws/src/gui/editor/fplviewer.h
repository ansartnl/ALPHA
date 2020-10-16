#ifndef FPLVIEWER_H
#define FPLVIEWER_H

#include "ui_fplviewer.h"
#include "abstractdialog.h"
#include "fpl.h"

#include <QtCore/QString>

#include <QtGui/QDialog>

#include <QtSql/QSqlRecord>

class QAbstractButton;

    /// Dialog display FPL in the detailed style and disallows editing
    class FplViewer : public AbstractDialog, public Ui::FplViewer
    {
        Q_OBJECT
    public:
        FplViewer(QWidget* pParent = 0);
        virtual ~FplViewer();

        void setFPL(const FPL &Fpl); /*!< Load FPL into dialog */
    protected:
        Q_SLOT
        void on_buttonBox_clicked(QAbstractButton* button);
    protected:
        virtual void changeEvent(QEvent *event);

    private:
        QPushButton *printButton;
    };
#endif // FPLVIEWER_H
