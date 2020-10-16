#ifndef CELLEDITORDLG_H
#define CELLEDITORDLG_H

#include <QDialog>

namespace Ui {
    class CellEditorDlg;
}

namespace Uim {

class CellEditorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CellEditorDlg(QWidget *parent = 0);
    ~CellEditorDlg();

    //! Set cell's text: row/column
    void setCellLabelText(const QString &text);

    //! Set row/column number.
    void setCellValue(int value);
    //! Retrieve current cell value: row/column number.
    int cellValue() const;

    //! Set span row/column occupies.
    void setSpanValue(int value);
    //! Retrieve span occupies.
    int spanValue() const;

    //! Static convenience function to get an integer cell value from the user.
    /*!
        \param [in] valueCell Row/column number
        \param [in,out] valueSpan Span value
    */
    static int getCell(QWidget *parent, const QString &title, const QString &labelText,
                       int valueCell, int &valueSpan, bool *ok = 0);

protected:
    virtual void changeEvent(QEvent * event);

private:
    Ui::CellEditorDlg *ui;
};

}   // namespace Uim

#endif // CELLEDITORDLG_H
