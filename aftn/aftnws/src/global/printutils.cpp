#include <QtCore/QVariant>

#include <QPainter>
#include <QPrinter>
#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QDateTimeEdit>

bool printWidget(QWidget *widget, QPrinter *printer)
{
  // Simplify widget
  QPainter painter(printer);

  // Check that there is a valid device to print to.
  if (!painter.isActive())
    return false;

  QList<QWidget*> widgetChildren = widget->findChildren<QWidget*> ();

  QWidget *parent = 0;
  QRect childRect;

  foreach (QWidget* currentChild, widgetChildren)
    {
      if (!currentChild->isVisible())
        continue;

      childRect = currentChild->frameGeometry();
      parent = currentChild->parentWidget();
      if (parent != widget)
        continue;

      // рисовать ли рамку?
      bool drawRect = true;
      QVariant prop = currentChild->property("text");
      QString text = currentChild->property("text").toString();

      //    while (parent != widget) {
      //      childRect.translate(parent->pos());
      //      parent = parent->parentWidget();
      //    }

      if (!currentChild->isEnabled())
        continue;

      if (qobject_cast<QLabel*>(currentChild))
        drawRect = false;

      if (QCheckBox *checkBox = qobject_cast<QCheckBox*> (currentChild)) {
        painter.save();
        if (checkBox->isChecked())
          painter.setBrush(QBrush(Qt::black));
        else
          painter.setBrush(QBrush());
        QRect checkRect(childRect.x() + 4, childRect.y() + childRect.height()
            / 2 - 3, 6, 6);
        painter.drawRect(checkRect);
        painter.restore();
        drawRect = false;
      }

      if (QComboBox* comboBox = qobject_cast<QComboBox*> (currentChild)) {
        if (comboBox->lineEdit()) {
          currentChild = (comboBox->lineEdit());
          text = comboBox->currentText();
        } else {
          text = comboBox->currentText();
        }
      }

      if (drawRect)
        painter.drawRect(childRect);

      painter.drawText(childRect, Qt::TextWordWrap | Qt::AlignVCenter
          | Qt::AlignHCenter, text);
    }
  return true;
}
