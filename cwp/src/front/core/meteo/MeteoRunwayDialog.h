#ifndef METEORUNWAYDIALOG_H
#define METEORUNWAYDIALOG_H

#include "ui_MeteoRunwayDialog.h"

#include "MeteoRunway.h"

#include <QDialog>

class MeteoRunwayDialog : public QDialog, Ui::MeteoRunwayDialog
{
    Q_OBJECT
public:
    explicit MeteoRunwayDialog(QWidget* parent = 0);
    virtual ~MeteoRunwayDialog();

    void setMeteo(const MeteoRunway& meteo);

    MeteoRunway meteo() const;

public slots:
    virtual void accept();

protected:
    bool validateMeteo();

protected:
    QString airportName;
};

#endif
