#ifndef EDITHEADERDIALOG_H
#define EDITHEADERDIALOG_H

#include "ui_EditHeaderDialog.h"

#include "EditHeaderInterface.h"

#include <QtCore/QAbstractTableModel>

class EditHeaderModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    EditHeaderModel(const QStringList &languages, QObject *parent = 0);

    void setItems(const EditHeaderItems &items);
    EditHeaderItems items() const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    void swap(const QModelIndex &index1, const QModelIndex &index2);

protected:
    EditHeaderItem & item(const QModelIndex &index);
    EditHeaderItem item(const QModelIndex &index) const;
    EditHeaderItem item(int row) const;
    QString text(int number, int language) const;

    bool isNumber(const QModelIndex &index) const;
    bool isText(const QModelIndex &index) const;
    bool isVisibility(const QModelIndex &index) const;

    bool isNumber(int column) const;
    bool isText(int column) const;
    bool isVisibility(int column) const;

protected:
    QStringList mLanguages;
    QMap < int, EditHeaderItem > mItems;
};

class EditHeaderDialog : public QDialog, private Ui::EditHeaderDialog
{
    Q_OBJECT
public:
    explicit EditHeaderDialog(QWidget *parent = 0);
    virtual ~EditHeaderDialog();

    void setEditHeaderInterface(QSharedPointer<EditHeaderInterface> &edit_interface);

    void setDialogName(const QString &name);

    EditHeaderModel* getModel(){
        return model;
    }
    //! Gets current selection Index for item mode , or returns invalid index
    QModelIndex getModelSelection();

protected:
    void saveSettings();
    void loadSettings();

protected slots:
    void onSelected(const QModelIndex &newIndex);

    void onUpButton();
    void onDownButton();

    void onAccepted();

protected:
    QSharedPointer<EditHeaderInterface> editInterface;
    EditHeaderModel *model;
    QString mName;
};

#endif // EDITHEADERDIALOG_H
