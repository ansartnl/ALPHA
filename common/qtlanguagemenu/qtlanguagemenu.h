#ifndef QTLANGUAGEMENU_H
#define QTLANGUAGEMENU_H

#include <QMenu>

/*!
  * \class QtLanguageMenu
  * \brief Языковое меню
  *
  * Данный класс предоставляет элемент графического пользовательского интерфеса - языковое меню.
  * Он автоматически загружает список доступных языков из класса QtTranslator и посылает этому
  * классу команду на изменение языка, при выборе соответствующего пункта меню.
  */

class QtLanguageMenu : public QMenu
{
    Q_OBJECT
public:
    QtLanguageMenu(QWidget *parent = 0);
    virtual ~QtLanguageMenu();

public slots:
    void reloadTranslations();

protected:
    virtual void changeEvent(QEvent* event);

private:
    void retranslateUi();
};

#endif // QTLANGUAGEMENU_H
