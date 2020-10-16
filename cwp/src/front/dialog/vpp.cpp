#include "vpp.h"
#include "ui_vpp.h"

#include <QTcpSocket>

Vpp::Vpp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vpp), mStatCount(3), mRunwayName(tr("Runway")),
    mIsDisabled(false)
{
    ui->setupUi(this);    

    mStats = new int[mStatCount];
    for(int i = 0; i < mStatCount; i++)
        mStats[i] = 0;

    connect(ui->btnRemPeople, SIGNAL(clicked()), this, SLOT(onRemovePeople()));
    connect(ui->btnAddPeople, SIGNAL(clicked()), this, SLOT(onAddPeople()));
    connect(ui->btnAddAuto, SIGNAL(clicked()), this, SLOT(onAddAuto()));
    connect(ui->btnRemAuto, SIGNAL(clicked()), this, SLOT(onRemoveAuto()));
    connect(ui->btnRemOther, SIGNAL(clicked()), this, SLOT(onRemoveOther()));
    connect(ui->btnAddOther, SIGNAL(clicked()), this, SLOT(onAddOther()));

    updateVpp();
}

Vpp::~Vpp()
{
    delete[] mStats;
    delete ui;
}

void Vpp::reset()
{
    for(int i = 0; i < mStatCount; i++)
        mStats[i] = 0;

    updateVpp();
}

void Vpp::onNewData(QString runway, int numPeople, int numAuto, int numOther)
{
    if(runway != mRunwayName)
        return;

    mStats[0] = qMax(numPeople, 0);
    mStats[1] = qMax(numAuto, 0);
    mStats[2] = qMax(numOther, 0);

    ui->labelPeopleCount->setText( QString::number(mStats[0]));
    ui->labelAutoCount->setText( QString::number(mStats[1]));
    ui->labelOtherCount->setText( QString::number(mStats[2]));

    updateVpp();
}

void Vpp::onRemovePeople()
{
    if(mStats[0] == 0)
        return;
    mStats[0]--;
    ui->labelPeopleCount->setText( QString::number(mStats[0]));
    emit peopleCount(-1);
    updateVpp();
}

void Vpp::onAddPeople()
{
    mStats[0]++;
    ui->labelPeopleCount->setText( QString::number(mStats[0]));
    emit peopleCount(1);
    updateVpp();
}

void Vpp::onRemoveAuto()
{
    if(mStats[1] == 0)
        return;
    mStats[1]--;
    ui->labelAutoCount->setText( QString::number(mStats[1]));
    emit autoCount(-1);
    updateVpp();
}

void Vpp::onAddAuto()
{
    mStats[1]++;
    ui->labelAutoCount->setText( QString::number(mStats[1]));
    emit autoCount(1);
    updateVpp();
}

void Vpp::onRemoveOther()
{
    if(mStats[2] == 0)
        return;
    mStats[2]--;
    ui->labelOtherCount->setText( QString::number(mStats[2]));
    emit otherCount(-1);
    updateVpp();
}

void Vpp::onAddOther()
{
    mStats[2]++;
    ui->labelOtherCount->setText( QString::number(mStats[2]));
    emit otherCount(1);
    updateVpp();
}

void Vpp::updateVpp()
{
    if(mIsDisabled)
    {
        ui->label_vpp->setText(tr("Widget unconfigured"));
        ui->backgroundView->setStyleSheet("background-color: #777777;");

        return;
    }

    bool isFree = true;
    for(int i = 0; i < mStatCount; i++)
    {
        if(mStats[i] != 0)
        {
            isFree = false;
            break;
        }
    }

    if(isFree)
    {
        ui->label_vpp->setText(mRunwayName + tr(" is free"));
        ui->backgroundView->setStyleSheet("background-color: #30751D;");
        //ui->label_vpp->setStyleSheet("background-color: #30751D;");
    }
    else
    {
        ui->label_vpp->setText(mRunwayName + tr(" is occupied"));
        ui->backgroundView->setStyleSheet("background-color: #F00000;");
        //ui->label_vpp->setStyleSheet("background-color: #F00000;");
    }

}

void Vpp::updateButtons()
{
    ui->btnRemPeople->setDisabled(mIsDisabled);
    ui->btnAddPeople->setDisabled(mIsDisabled);
    ui->btnAddAuto->setDisabled(mIsDisabled);
    ui->btnRemAuto->setDisabled(mIsDisabled);
    ui->btnRemOther->setDisabled(mIsDisabled);
    ui->btnAddOther->setDisabled(mIsDisabled);
}

void Vpp::Translate()
{
    ui->retranslateUi(this);
    updateVpp();
}

void Vpp::UpdateXFont()
{
}
