#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "StdinReader.h"

#include <QtCore/QObject>
#include <QtCore/QString>

#include <QtGui/QMainWindow>

#include <QtNetwork/QAbstractSocket>

class QWidget;
class QMdiArea;
class QAction;
class QLineEdit;
class QComboBox;
class QPushButton;
class QSettings;
class QTranslator;
class QMdiSubWindow;
class QSessionManager;
class QModelIndex;

namespace Ui {
  class MainWindow;
}

namespace db {
  class CDB;
}

class QtToolBarManager;

/*!
 Main window class.

 Object manager for application GUI objects
 Actions framework (preview-print-undo-redo-cut-copy-paste...)
 QtToolbarDialog for customize toolbars

 \warning Used Qt PIMPL
 */
class MainWindowPrivate;
class MainWindow: public QMainWindow
{
  Q_OBJECT

  Q_DECLARE_PRIVATE(MainWindow)
    void init();
  protected:
    const QScopedPointer<MainWindowPrivate> d_ptr;
    MainWindow(MainWindowPrivate& dd, QWidget* parent);

    const QScopedPointer<Ui::MainWindow> ui;

    StdinReader *stdinReader;
  public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

    virtual void saveSettings();
    virtual void restoreSettings();

    void statusAFTNChanged(bool bConnected);

  protected slots:

    void onStdin(const QString &str);
    /**
     * @brief Relogin
     *
     * @fn on_actionLogin_triggered
     */
    void on_actionLogin_triggered();

    /**
     * @brief Create telegram
     *
     * @fn on_actionCreate_telegram_triggered
     */
    void on_actionCreate_telegram_triggered();

    /**
     * @brief Address book
     *
     * @fn on_actionAddress_book_triggered
     */
    void on_actionAddress_book_triggered();

    void browserStatusChanged(const QString& newStatus);

    /**
     * @brief Handler for close button on mdi child window tabs
     *
     * @fn closeMdiTab
     * @param int
     */
    void closeMdiTab(int);

    /**
     * @brief When activate mdi child window, update it data
     *
     * @fn onSubWindowActivated
     * @param subWindow
     */
    void onSubWindowActivated(QMdiSubWindow* subWindow);

    /**
     * @brief Exec modal dialog for configuring toolbars
     *
     * @fn configureToolBars
     */
    void configureToolBars();

    /**
     * @brief When user clicked on some application GUI object
     *
     * @fn databaseObjectTriggered
     * @param index
     */
    void databaseObjectTriggered(const QModelIndex& index);
    void databaseObjectTriggered(const QString& id);
    void OnShutdown(QSessionManager& manager);

    void onPrintAction();
    void on_actionFont_triggered();

    void on_actionAuto_answer_on_error_triggered();
    void onSetCheck_actionAuto_answer_on_error();
  protected:
    /**
     * @brief Init some variables, timers on first window show
     *
     * @fn showEvent
     * @param event
     */
    void showEvent(QShowEvent* event);

    /**
     * @brief Save main window state
     *
     * @fn maybeSave
     * @return bool
     */
    virtual bool maybeSave();
    virtual void closeEvent(QCloseEvent* event);

    virtual void timerEvent(QTimerEvent *event);

    virtual void changeEvent(QEvent * event);

    virtual QStringList saveObjectsState() const;
    virtual void restoreObjectsState(const QStringList& objects);
};

#include <QtGui/QToolButton>
/**
 For internal use only
 If action linked with toolbutton is enabled, then draw red font, else draw default font.
 */
class RedToolButton: public QToolButton
{
  Q_OBJECT
  public:
    RedToolButton();

  protected:
    virtual void paintEvent(QPaintEvent *event);
};

#endif // MAINWINDOW_H
