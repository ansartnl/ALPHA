#include "GUIListConstructor.h"
#include "ui_GUIListConstructor.h"
#include <QColorDialog>
#include "ListConstructor.h"
#include "main.h"

CGUIListConstructor::CGUIListConstructor(QWidget *parent) :
    QDialog(parent),
    sResult("none"),
    ui(new Ui::CGUIListConstructor),
    pEditDlg(0),
    m_WarningColor(Qt::yellow),
    m_DangerColor(Qt::red)
{
    ui->setupUi(this);    

    ui->lists_combo->addItems(
                QStringList()   << QString("ACC")
                                << QString("SIL_ACC")
                                << QString("APP")
                                << QString("SIL_APP_ARR")
                                << QString("SIL_APP_DEP")
                                << QString("ACT_IN")
                                << QString("ACT_OUT")
                                << QString("ACT_DEP")
                                << QString("DAILY_PLAN")
                                << QString("DAILY_PLAN_ANO")
                                << QString("COST"));
    ui->lists_combo->setCurrentIndex(0);

    translate();
    connect(ui->btn_warning_color, SIGNAL(clicked()), this, SLOT(OnBtnWarning()));
    connect(ui->btn_danger_color, SIGNAL(clicked()), this, SLOT(OnBtnDanger()));
    connect(ui->btn_default_all, SIGNAL(clicked()), this, SLOT(OnBtnAllDefault()));
    connect(ui->btn_default_current, SIGNAL(clicked()), this, SLOT(OnBtnCurrentDefault()));
    connect(ui->lists_combo, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnIndexChanged(QString)));

    SetupNewDialogWidget();
    loadUserData();
}

CGUIListConstructor::~CGUIListConstructor()
{
    delete ui;
}

void CGUIListConstructor::loadUserData()
{
    globalSettings()->beginGroup(sUserName);
    if(globalSettings()->allKeys().contains(QString("DangerColor"))){
        QDataStream stream(
                    QByteArray::fromHex(
                        globalSettings()->value(QString("DangerColor")).toByteArray()));
        stream >> m_DangerColor;
    }
    else{
        m_DangerColor = Qt::red;
    }

    if(globalSettings()->allKeys().contains(QString("WarningColor"))){
        QDataStream stream(
                    QByteArray::fromHex(
                        globalSettings()->value(QString("WarningColor")).toByteArray()));
        stream >> m_WarningColor;
    }
    else{
        m_WarningColor = Qt::yellow;
    }

    globalSettings()->endGroup();

    loadListData();

}

void CGUIListConstructor::loadListData()
{
    QString group = sUserName + tr("/") + ui->lists_combo->currentText();
    QSharedPointer<CListConstructor> lc(new CListConstructor(group));
    lc->loadItems();
    QSharedPointer<EditHeaderInterface> edit_interface = qSharedPointerDynamicCast<EditHeaderInterface>(lc);
    pEditDlg->setEditHeaderInterface(edit_interface);
}

void CGUIListConstructor::SetupNewDialogWidget()
{
    if (pEditDlg)
    {
        pEditDlg->blockSignals(true);
        pEditDlg->reject();
        delete pEditDlg;

        pEditDlg = new EditHeaderDialog(this);
        ui->widget->layout()->addWidget((QWidget*)pEditDlg);
    }
    else
    {
        pEditDlg = new EditHeaderDialog(this);
        QVBoxLayout* childLayout = new QVBoxLayout(ui->widget);
        childLayout->addWidget((QWidget*)pEditDlg);
        ui->widget->setLayout(childLayout);
    }

    connect(pEditDlg, SIGNAL(rejected()), this, SLOT(onRejected()));
    connect(pEditDlg, SIGNAL(accepted()), this, SLOT(onAccepted()));

    pEditDlg->setDialogName(tr("Plan Lists Editor"));
}

void CGUIListConstructor::translate()
{
    ui->btn_warning_color->setText(tr("Set Warning color"));
    ui->btn_danger_color->setText(tr("Set Danger color"));
    ui->btn_default_all->setText(tr("Set all to default"));
    ui->btn_default_current->setText(tr("Set line to default"));
}

void CGUIListConstructor::OnBtnWarning()
{
    QColor color = QColorDialog::getColor(m_WarningColor, this);
    if (color.isValid())
        m_WarningColor = color;
}

void CGUIListConstructor::OnBtnDanger()
{
    QColor color = QColorDialog::getColor(m_DangerColor, this);
    if (color.isValid())
        m_DangerColor = color;
}

void CGUIListConstructor::OnBtnAllDefault()
{
    EditHeaderModel* model = pEditDlg->getModel();
    EditHeaderItems items = model->items();

    QModelIndex idx;
    for (int i = 0; i < items.count(); i++){
        idx = model->index(i, 1);
        model->setData(idx, QVariant(items[i].identifier));
    }
}

void CGUIListConstructor::OnBtnCurrentDefault()
{
    EditHeaderModel* model = pEditDlg->getModel();
    EditHeaderItems items = model->items();
    QModelIndex idx = pEditDlg->getModelSelection();

    if (idx.isValid())
        model->setData(idx, QVariant(items[idx.row()].identifier));
}

void CGUIListConstructor::onAccepted()
{
    globalSettings()->beginGroup(sUserName);

    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);
    stream << m_DangerColor;
    globalSettings()->setValue(QString("DangerColor"), bytes.toHex());

    QByteArray bytes2;
    QDataStream stream2(&bytes2, QIODevice::ReadWrite);
    stream2 << m_WarningColor;
    globalSettings()->setValue(QString("WarningColor"), bytes2.toHex());

    globalSettings()->endGroup();

    sResult = ui->lists_combo->currentText();
    close();
}

void CGUIListConstructor::onRejected()
{    
    close();
}

void CGUIListConstructor::OnIndexChanged(QString sCur)
{
    SetupNewDialogWidget();
    loadListData();    
}
