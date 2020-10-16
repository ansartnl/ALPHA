#ifndef ACTINLISTTABLEMODEL_H
#define ACTINLISTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

#include <QSet>
#include <QTime>

struct Act
{
	QString eq;
	QString acid;
	QString code;
	QString cop;
	QTime eto;
	int pel;
	QString adep;
	QString dest;

	QString cs;
	QString ns;

	int tfl;
	int rfl;
};

class ActTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
	enum ActType
	{
		act_in = 0,
        act_out,
        act_dep
	};

    typedef bool(*CalcNextSector)(const QString &cop_in, int pel);

    explicit ActTableModel(ActType act_type, CalcNextSector calc_next_sector = 0, QObject *parent = 0,QStringList cop_list= {});

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	void setActs(const QList<Act> &acts);
	void addAct(const Act &act_in);
	Act act(int row) const;
	bool checkData(int row);

    ActType type() const { return act_type_; }

public slots:
	void setNonEditableFields(const QSet<int> &non_editable_fields);
    void clearErrors() { invalid_fields_.clear(); }

private:
	Q_DISABLE_COPY(ActTableModel)

	ActType act_type_;
	QList<Act> acts_;


    QStringList copList;

    QStringList header_;
	QSet<int> non_editable_fields_;
    QSet<int> invalid_fields_;

    CalcNextSector calc_next_sector_;
};

#endif // ACTINLISTTABLEMODEL_H
