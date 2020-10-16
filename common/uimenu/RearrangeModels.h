#ifndef _REARRANGE_MODELS_H__
#define _REARRANGE_MODELS_H__

#include "UiItems.h"
#include "AbstractUiModel.h"

#include <QtCore/QAbstractItemModel>


namespace Uim {

//! Base model representation.
class BaseUiModel : public QAbstractItemModel
{
public:
    static const int userRoleText = Qt::UserRole+9;
    static const int userRoleStyle = Qt::UserRole+10;
    static const int userRoleItemType = Qt::UserRole+11;
    static const int userRoleToolTip = Qt::UserRole+12;

    enum ItemType
    {
        Separator,
        Group,
        Action,
        Toolbutton
    };

    //! Set new data reference.
    void setData(const TGroupItem &groupItem);

    //! Insert separator to specified position.
    bool insertSeparator(int row);
    //! Insert group to specified position.
    bool insertGroup(int row);
    //! Insert action to specified position.
    virtual bool insertAction(int row, const QString &actionClassName = QString(),
                              bool isSubmenu = false) = 0;

    //! Remove item from specified position.
    bool removeRow(int row);

    //! Swap 2 rows.
    void swapRows(int i, int j);

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

protected:
    explicit BaseUiModel(const AbstractUiModel *modelCmd, QObject *parent = 0);

protected:
    /** Current list of actions */
    TGroupItem              mGroupItem;
    /** Ui actions model */
    const AbstractUiModel   *mModelCmd;

    /** Preloaded transparent icon */
    static QIcon            iconTransparent;
};


//! Model for representing items for sub menu or menu bar
class MenuUiModel : public BaseUiModel
{
public:
    explicit MenuUiModel(const AbstractUiModel *modelCmd, QObject *parent = 0)
        : BaseUiModel(modelCmd, parent)
    {}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    //! Insert action to specified position.
    virtual bool insertAction(int row, const QString &actionClassName = QString(),
                              bool isSubmenu = false);
};


//! Model for representing items for toolbar
class ToolbarUiModel : public BaseUiModel
{
public:
    static const int userRoleRow = Qt::UserRole+20;
    static const int userRoleColumn = Qt::UserRole+21;
    static const int userRoleRowSpan = Qt::UserRole+22;
    static const int userRoleColumnSpan = Qt::UserRole+23;
    static const int userRoleAlignment = Qt::UserRole+24;
    static const int userRoleIconSize = Qt::UserRole+25;
    static const int userRoleSizePolicy = Qt::UserRole+26;

    enum SizePolicy
    {
        Fixed,
        Minimum
    };

    explicit ToolbarUiModel(const AbstractUiModel *modelCmd, QObject *parent = 0)
        : BaseUiModel(modelCmd, parent)
    {}

    //! Set position in grid layout for specified item.
    /*!
        \param index Item position
        \param row Row index in grid layout
        \param row Column index in grid layout
    */
    void setGridPosition(const QModelIndex &index, int row, int column);

    //! Retrieve list of available alignments.
    QStringList alignmentList() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //! Insert toolbutton to specified position.
    virtual bool insertAction(int row, const QString &actionClassName = QString(),
                              bool isSubmenu = false);
};

}   // namespace Uim

#endif // _REARRANGE_MODELS_H__
