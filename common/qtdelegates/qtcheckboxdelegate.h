#ifndef QTCHECKBOXDELEGATE_H
#define QTCHECKBOXDELEGATE_H

#include <QtCore/QVariant>

#include <QStyledItemDelegate>
/*!
  Делегат для bool значений модели. Не создает виджета.
  \ingroup delegates
  \brief Делегат чекбокса
* @class QtCheckBoxDelegate qtcheckboxdelegate.h "src/kernel/delegates/checkboxdelegate.h"
*/
class QtCheckBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT

  QRect checkRect
    (const QStyleOptionViewItem &option,
     const QRect &bounding) const;

  Q_PROPERTY (QVariant trueValue READ trueValue WRITE setTrueValue)
  Q_PROPERTY (QVariant falseValue READ falseValue WRITE setFalseValue)
public:

  QtCheckBoxDelegate(QObject* parent = 0, const QVariant& trueValue = true, const QVariant& falseValue = false);
  virtual ~QtCheckBoxDelegate(){}

  QWidget* createEditor (QWidget* parent,
      const QStyleOptionViewItem& option,
      const QModelIndex& index ) const;

  void paint (QPainter* painter,
       const QStyleOptionViewItem& option,
       const QModelIndex& index) const;

  QString displayText(const QVariant &value
                      , const QLocale &locale) const;

  bool editorEvent (QEvent* ev,
       QAbstractItemModel* model,
       const QStyleOptionViewItem& option,
       const QModelIndex& index);

  QVariant trueValue() const;
  void setTrueValue(const QVariant& value);

  QVariant falseValue() const;
  void setFalseValue(const QVariant& value);

private:
  QVariant _trueValue;
  QVariant _falseValue;
};

#endif
