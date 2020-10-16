#include "acttablemodel.h"

#include <QColor>
#include <QDateTime>
#include <QMessageBox>
#include "main.h"

ActTableModel::ActTableModel(ActType act_type, CalcNextSector calc_next_sector, QObject *parent,QStringList cop_list) :
    QAbstractTableModel(parent),
	act_type_(act_type),
    calc_next_sector_(calc_next_sector),
    copList(cop_list)
{
	if (act_in == act_type_) {
		header_ << "EQ" << "ACID" << "CODE" << "COP" << "ETO" << "PEL" << "ADEP" << "DEST";
#ifdef QT_DEBUG
		header_ << "CS" << "NS";
#endif
    } else if (act_out == act_type_) {
		header_ << "EQ" << "ACID" << "CODE" << "COP" << "ETO" << "TFL" << "RFL";
    } else if (act_dep == act_type_) {
        header_ << "EQ" << "ACID" << "CODE" << "ETD" << "ADEP" << "DEST";
    }
}

int ActTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

	return acts_.count();
}

int ActTableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)

	return header_.size();
}

QVariant ActTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

	if (acts_.count() <= index.row())
        return QVariant();

    //const Act &act = acts_.at(index.row());
    Act act = acts_[index.row()];

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
	} else if ((role == Qt::DisplayRole) || (role == Qt::EditRole)) {
        switch (index.column()) {
		case 0: return act.eq;
		case 1: return act.acid.toUpper();
		case 2: return act.code;
        case 3:
        {
            QString sText;
            switch(act_type_)
            {
            case act_in:
            case act_out:
                sText = act.cop.toUpper();
                break;
            case act_dep:
                sText = act.eto.toString("hh:mm");
                break;
            }

            return sText;
        }
        case 4:
        {
            QString sText;
            switch(act_type_)
            {
            case act_in:
            case act_out:
                sText = act.eto.toString("hh:mm");
                break;
            case act_dep:
                if(act.adep.isEmpty())
                    if(!networkSettings()->value("meteo/airport").toString().isEmpty())
                    {
                        act.adep.clear();
                        act.adep.append(networkSettings()->value("meteo/airport").toString().toUpper());
                    }
                sText = act.adep.toUpper();
                break;
            }

            return sText;
        }
        case 5:
        {
            QString sText;
            switch(act_type_)
            {
            case act_in:
                sText = (act.pel == 0 ? QString() : QString("%1").arg(act.pel));
                break;
            case act_out:
                sText = (act.tfl == 0 ? QString() : QString("%1").arg(act.tfl));
                break;
            case act_dep:
                sText = act.dest.toUpper();
                break;
            }

            return sText;
        }
        case 6:
        {
            QString sText;
            switch(act_type_)
            {
            case act_in:
                sText = act.adep.toUpper();
                break;
            case act_out:
                sText = (act.rfl == 0 ? QString() : QString("%1").arg(act.rfl));
                break;
            case act_dep:
                break;
            }

            return sText;
        }
        case 7: return act.dest.toUpper();
		case 8: return act.cs;
		case 9: return act.ns;
        default: return QString("N/A");
        }
    } else if (Qt::BackgroundColorRole == role) {
        return invalid_fields_.find(index.column()) == invalid_fields_.end() ? QColor(Qt::white) : QColor(Qt::red);
    }

    return QVariant();
}

bool ActTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;

	if (acts_.count() <= index.row())
		return false;

	if (role == Qt::EditRole) {
        Act act = acts_[index.row()];

		switch (index.column()) {
		case 0:
		act.eq = value.toString();
		break;
		case 1:
		act.acid = value.toString().toUpper();
		break;
		case 2:
		act.code = value.toString();
		break;
		case 3:
        {
            switch(act_type_)
            {
            case act_in:
            case act_out:
                act.cop = value.toString().toUpper();
                break;
            case act_dep:
                act.eto = QTime::fromString(value.toString(), "hh:mm");
                break;
            }
            break;
        }
		case 4:
        {
            switch(act_type_)
            {
            case act_in:
            case act_out:
                act.eto = QTime::fromString(value.toString(), "hh:mm");
                break;
            case act_dep:
                act.adep = value.toString().toUpper();
                break;
            }
            break;
        }
        case 5:
        {
            switch(act_type_)
            {
            case act_in:
                act.pel = value.toInt();
            case act_out:
                act.tfl = value.toInt();
                break;
            case act_dep:
                act.dest = value.toString().toUpper();
                break;
            }
            break;
        }
        case 6:
        {
            switch(act_type_)
            {
            case act_in:
                act.adep = value.toString().toUpper();
            case act_out:
                act.rfl = value.toInt();
                break;
            case act_dep:
                break;
            }
            break;
        }
		case 7:
        act.dest = value.toString().toUpper();
		break;
		case 8:
		act.cs = value.toString();
		break;
		case 9:
		act.ns = value.toString();
		break;
		default: return false;
		}

        acts_[index.row()] = act;
    }

	emit dataChanged(index, index);

	return true;
}

QVariant ActTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
        return section;
    
    return header_.at(section);
}

bool ActTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        header_[section] = value.toString();
        emit headerDataChanged(orientation, section, section);
        return true;
    }

    return false;
}

Qt::ItemFlags ActTableModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if (non_editable_fields_.find(index.column()) == non_editable_fields_.end())
		flags |= Qt::ItemIsEditable;
	return flags;
}

void ActTableModel::setActs(const QList<Act> &acts)
{
    beginResetModel();
	acts_ = acts;
	endResetModel();
}

void ActTableModel::addAct(const Act &act)
{
    beginResetModel();
	acts_.push_back(act);
	endResetModel();
}

Act ActTableModel::act(int row) const
{
	if (row < acts_.count() && row >= 0)
		return acts_[row];
    
    return Act();
}

bool ActTableModel::checkData(int row)
{
	if ((row >= acts_.count()) || (row < 0))
		return false;

    invalid_fields_.clear();

	const Act &act = acts_.at(row);

    static const QStringList equipments(QStringList() << "w" << "y" << "wy" << "");
    if (!equipments.contains(act.eq))
        invalid_fields_.insert(0);

    if ((act.acid.length() > 7) || act.acid.isEmpty())
        invalid_fields_.insert(1);

	if ((act.code.length() > 5) || (act.code.length() < 4))
        invalid_fields_.insert(2);

    if (act_dep != act_type_) {
        if ( ((act.cop.length() > 5) || act.cop.isEmpty()) ||
             (copList.size() && !copList.contains(act.cop)) )
        {
            invalid_fields_.insert(3);
        }
        if (act.eto.toString().isEmpty())
            invalid_fields_.insert(4);
    } else {
        if (act.eto.toString().isEmpty())
            invalid_fields_.insert(3);
        //if (act.adep.length() != 4)
        //    invalid_fields_.insert(4);

    }

    if (act_in == act_type_) {
		if ((act.pel > 999) || act.pel < 5)
            invalid_fields_.insert(5);
    } else if (act_out == act_type_) {
        if ((act.tfl > 999) || act.tfl < 5) {
            //QMessageBox::critical(0, tr("TFL"), tr("Field TFL invalid!"));
            invalid_fields_.insert(5);
            //return false;
        }
    } else if (act_dep == act_type_) {
        /*if (act.dest.length() != 4) {
            //QMessageBox::critical(0, tr("DEST"), tr("Field DEST invalid!"));
            invalid_fields_.insert(5);
            //return false;
        }*/
    }

    if (act_in == act_type_) {
        /*if (act.adep.length() != 4) {
            //QMessageBox::critical(0, tr("ADEP"), tr("Field ADEP invalid!"));
            invalid_fields_.insert(6);
            //return false;
        }*/
    } else if (act_out == act_type_) {
        /*if ((act.rfl > 999) || act.rfl < 5)
            invalid_fields_.insert(6);*/
    }

    if (act_in == act_type_) {
        /*if (act.dest.length() != 4) {
            //QMessageBox::critical(0, tr("DEST"), tr("Field DEST invalid!"));
            invalid_fields_.insert(7);
            //return false;
        }*/

        if (calc_next_sector_ && !calc_next_sector_(act.cop, act.pel))
            invalid_fields_.insert(3);
    }

     if (act_in == act_type_) {
         if (act.adep.contains(QRegExp("[0-9]{1,1}")))
            invalid_fields_.insert(6);
         if (act.dest.contains(QRegExp("[0-9]{1,1}")))
            invalid_fields_.insert(7);
     }

     if (act_dep == act_type_) {
         if (act.adep.contains(QRegExp("[0-9]{1,1}")))
            invalid_fields_.insert(4);
         if (act.dest.contains(QRegExp("[0-9]{1,1}")))
            invalid_fields_.insert(5);
     }

    if (!invalid_fields_.empty()) {
        beginResetModel();
        endResetModel();
        return false;
    }

    return invalid_fields_.empty() ? true : false;
}

void ActTableModel::setNonEditableFields(const QSet<int> &non_editable_fields)
{
	non_editable_fields_ = non_editable_fields;
}
