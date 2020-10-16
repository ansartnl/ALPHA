#include "FplParser.h"

#include <QtDebug>

#include <QtCore/QVariantMap>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>

namespace {
    QString qstrBaseYear()
    {
        QString year = QString::number(QDateTime::currentDateTime().toUTC().date().year());

        return year.left(year.size() - 2);
    }

    int qintBaseYear()
    {
        return (QDateTime::currentDateTime().toUTC().date().year() / 100) * 100;
    }
}

namespace fplparser {

    // анализ поля типа 3
    // Тип, номер сообщения и исходные данные
    void parseItem3(const QString& field, QVariantMap& map)
    {
        QRegExp rexp("(ALR|RCF|CPL|EST|CDN|ACP|LAM|RQP|RQS|SPL|CHG|CNL|DLA|DEP|ARR|FPL)" // a) Индекс типа сообщения
                "(\\w{1,4}[/]\\w{1,4}\\d{3})?" // b) Номер сообщения
                "(\\w{1,4}[/]\\w{1,4}\\d{3})?\\n?"); // c) Исходные данные

        if (rexp.exactMatch(field))
        {
            QVariantMap item;

            item["a"] = rexp.cap(1);

            if (!rexp.cap(2).isEmpty())
                item["b"] = rexp.cap(2);

            if (!rexp.cap(3).isEmpty())
                item["c"] = rexp.cap(3);

            map["item3"] = item;
        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 3");// error handling
        }
    }


    void parseItem5(const QString& field, QVariantMap& map)
    {
        QRegExp rexp("(INCERFA|ALERFA|DETRESFA)" // a) Аварийная стадия
                "/"
                "(\\w{8})"// b) Составитель сообщения
                "/"
                "([\\w\\s]+)"); // c) Характер аварийного положения

        if (rexp.exactMatch(field))
        {
            QVariantMap item;

            item["a"] = rexp.cap(1);
            item["b"] = rexp.cap(2);
            item["c"] = rexp.cap(3);
            map["item5"] = item;
        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 5");// error handling
        }
    }

    QString composeItem5(const QVariantMap& binMessage)
    {
        QString message;
        if (binMessage.contains("item5"))
        {
            message.append(binMessage["item5"].toMap()["a"].toString());
            message.append("/");
            message.append(binMessage["item5"].toMap()["b"].toString());
            message.append("/");
            message.append(binMessage["item5"].toMap()["c"].toString());
        }
        else
        {
            return QString::null;
        }
        return message;
    }


        if (rexp.exactMatch(field.simplified()))
        {
            QString aircraftCount = rexp.cap(1).isEmpty() ? QString("1") : rexp.cap(1);

            QVariantMap item;

            item["a"] = aircraftCount;

            if (!rexp.cap(2).isEmpty())
                item["b"] = rexp.cap(2);
            if (!rexp.cap(3).isEmpty())
                item["c"] = rexp.cap(3);

            map["item9"] = item;
        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 9");// error handling
        }
    }

    QString composeItem9(const QVariantMap& binMessage)
    {
        QString message;
        if (binMessage.contains("item9"))
        {
            if (binMessage["item9"].toMap()["a"].toInt() > 1)
            {
                message.append(binMessage["item9"].toMap()["a"].toString());
            }
            message.append(binMessage["item9"].toMap()["b"].toString());
            message.append("/");
            message.append(binMessage["item9"].toMap()["c"].toString());
        }
        else
        {
            return QString::null;
        }
        return message;
    }

    void parseItem10(const QString& field, QVariantMap& map)
    {
        QRegExp rexp("(\\w+)" //      a) Средства радиосвязи, навигационные средства и средства захода на посадку
                "/"
                "(\\w+)" // b) Оборудование наблюдения
                "\\n?");

        if (rexp.exactMatch(field.simplified()))
        {
            QVariantMap item;

            item["a"] = rexp.cap(1);

            if (!rexp.cap(2).isEmpty())
                item["b"] = rexp.cap(2);

            map["item10"] = item;

        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 10");// error handling
        }
    }

    QString composeItem10(const QVariantMap& binMessage)
    {
        QString message;
        if (binMessage.contains("item10"))
        {
            message.append(binMessage["item10"].toMap()["a"].toString());
            message.append("/");
            message.append(binMessage["item10"].toMap()["b"].toString());
        }
        else
        {
            return QString::null;
        }
        return message;
    }

    void parseItem13(const QString& field, QVariantMap& map)
    {
        QRegExp rexp("(\\w{4})" //a) Аэродром вылета
                "(\\d{4})?" //b) Время
                "\\n?");

        if (rexp.exactMatch(field.simplified()))
        {
            QVariantMap item;

            item["a"] = rexp.cap(1);

            if (!rexp.cap(2).isEmpty())
            {
                item["b"] = rexp.cap(2);
            }

            map["item13"] = item;
        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 13");// error handling
        }
    }

    QString composeItem13(const QVariantMap& binMessage)
    {
        QString message;
        if (binMessage.contains("item13"))
        {
            message.append(binMessage["item13"].toMap()["a"].toString());
            if (binMessage["item13"].toMap().contains("b"))
            {
                message.append(binMessage["item13"].toMap()["b"].toString());
            }
        }
        else
        {
            return QString::null;
        }
        return message;
    }

    void parseItem14(const QString& field, QVariantMap& map)
    {
        QRegExp rexp("(\\w+)" //a) Пограничный пункт
                "/" // ДЕЛИТЕЛЬНАЯ КОСАЯ ЧЕРТА
                "(\\d{4})" //b) Время пролета пограничного пункта
                "([FA]\\d{3}|[SM]\\d{4}|VFR)" //c) Разрешенный уровень пролета
                "([FA]\\d{3}|[SM]\\d{4}|VFR)?" //d) Дополнительные данные о пролете
                "(\\w)?" //e) Условия пролета
                "\\n?");

        if (rexp.exactMatch(field.simplified()))
        {
            QVariantMap item;

            item["a"] = rexp.cap(1);
            item["b"] = rexp.cap(2);
            item["c"] = rexp.cap(3);

            if (!rexp.cap(4).isEmpty())
                item["d"] = rexp.cap(4);
            if (!rexp.cap(5).isEmpty())
                item["e"] = rexp.cap(5);

            map["item14"] = item;
        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 14");// error handling
        }
    }

            {
                message.append(" ");
                message.append(binMessage["item17"].toMap()["c"].toString());
            }
        }
        else
        {
            return QString::null;
        }
        return message;
    }

    void parseItem18(const QString& field, QVariantMap& map)
    {
        QRegExp rexp("(.+)"
            "\\n?");

        if (rexp.exactMatch(field.simplified()))
        {
            QVariantMap item;
            item["a"] = rexp.cap(1);

            if (rexp.cap(1) != "0")
            {
                QString data = rexp.cap(1);

                QRegExp splitter("(\\w{,4})/");

                int i = 0;
                QString current = data.section(splitter, i, i, QString::SectionIncludeLeadingSep);

                QRegExp section("(\\w{,4})" //a
                        "/"
                        "(.+)" //b
                        "\\n?");

                /// Workaround bug with QString::section() in windows
                int fieldCount = data.split(splitter).count();

                while (fieldCount)
                {
                    --fieldCount;

                    if (section.exactMatch(current))
                    {
                        QString fieldId = section.cap(1);
                        QString newFieldValue = section.cap(2);

                        item[fieldId] = newFieldValue.simplified();

                        if (!fieldCount)
                            break;
                    }
                    ++i;
                    current = data.section(splitter, i, i, QString::SectionIncludeLeadingSep);
                }
            }

            map["item18"] = item;
        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 18");// error handling
        }
    }

    QString composeItem18(const QVariantMap& binMessage)
    {
        QString message;
        if (binMessage.contains("item18"))
        {
            QString middleresult;
            QVariantMap item18 = binMessage["item18"].toMap();

            QVariantMap::ConstIterator it = item18.constBegin();
            QVariantMap::ConstIterator end = item18.constEnd();

            for (; it != end; ++it)
            {
                if (it.key() == "a")
                    continue;

                middleresult.append(it.key());
                middleresult.append("/");
                middleresult.append(it.value().toString().simplified());
                middleresult.append(" ");
            }
            message.append(middleresult.simplified());
        }
        else
        {
            return QString("0");
        }
        return message;
    }

    void parseItem19(const QString& field, QVariantMap& map)
    {
        QRegExp rexp("(.+)"
            "\\n?");

        if (rexp.exactMatch(field.simplified()))
        {
            QVariantMap item;
            item["a"] = rexp.cap(1);

            map["item19"] = item;
        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 19");// error handling
        }
    }

    QString composeItem19(const QVariantMap& binMessage)
    {
        QString message;
        if (binMessage.contains("item19"))
        {
            message.append(binMessage["item19"].toMap()["a"].toString());
        }
        else
        {
            return QString::null;
        }
        return message;
    }

    void parseItem20(const QString& field, QVariantMap& map)
    {
        QRegExp rexp("(.+)" //all
                "\\n?");

        if (rexp.exactMatch(field.simplified()))
        {
            QVariantMap item;
            item["a"] = rexp.cap(1);

            map["item20"] = item;
        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 20");// error handling
        }
    }

    QString composeItem20(const QVariantMap& binMessage)
    {
        QString message;
        if (binMessage.contains("item20"))
        {
            message.append(binMessage["item20"].toMap()["a"].toString());
        }
        else
        {
            return QString::null;
        }
        return message;
    }

    void parseItem21(const QString& field, QVariantMap& map)
    {
        QRegExp rexp("(\\d{4})" //a
                "\\s"
                "([\\d\\.]{5})" //b
                "\\s"
                "(\\w+)" //c
                "\\s"
                "(\\d{4})" //d
                "\\s"
                "(.+)" //e and f
                "\\n?");

        if (rexp.exactMatch(field.simplified()))
        {
            QVariantMap item;
            item["a"] = rexp.cap(1);
            item["b"] = rexp.cap(2);
            item["c"] = rexp.cap(3);
            item["d"] = rexp.cap(4);
            item["ef"] = rexp.cap(5);

            map["item21"] = item;
        }
        else
        {
            map["errors"] = (map["errors"].toStringList() << "Invalid item 21"); // error handling
        }
    }

    QString composeItem21(const QVariantMap& binMessage)
    {
        QString message;
        if (binMessage.contains("item21"))
        {
            message.append(binMessage["item21"].toMap()["a"].toString());
            message.append(" ");
            message.append(binMessage["item21"].toMap()["b"].toString());
            message.append(" ");
            message.append(binMessage["item21"].toMap()["c"].toString());
            message.append(" ");
            message.append(binMessage["item21"].toMap()["d"].toString());
            message.append(" ");
            message.append(binMessage["item21"].toMap()["ef"].toString());
        }
        else
        {
            return QString::null;
        }
        return message;
    }

    void parseItem22(const QString& field, QVariantMap& map)
    {
        QRegExp splitter("-\\d{1,2}/");

        QRegExp rexp("-(\\d{1,2})" //a
                "/"
                "(.+)" //b
                "\\n?");
        int i = 1;
        QString current = field.section(splitter, i, i, QString::SectionIncludeLeadingSep);

        // "Workaround bug with QString::section() in windows"
        int fieldCount = field.split(splitter).count() - 1;

        QVariantMap changedMap;

        while (fieldCount)
        {
            --fieldCount;

            if (rexp.exactMatch(current))
            {
                int fieldNumber = rexp.cap(1).toInt();
                QString newFieldValue = rexp.cap(2);

                QVariantList items = changedMap["list"].toList();
                items.append(fieldNumber);
                changedMap["list"] = items;

                switch (fieldNumber)
                {
                case 3:
                    break;
                case 5:
                    break;
                case 7:
                    parseItem7(newFieldValue, changedMap);
                    break;
                case 8:
                    parseItem8(newFieldValue, changedMap);
                    break;
                case 9:
                    parseItem9(newFieldValue, changedMap);
                    break;
                case 10:
                    parseItem10(newFieldValue, changedMap);
                    break;
                case 13:
                    parseItem13(newFieldValue, changedMap);
                    break;
                case 14:
                    parseItem14(newFieldValue, changedMap);
                    break;
                case 15:
                    parseItem15(newFieldValue, changedMap);
                    break;
                case 16:
                    parseItem16(newFieldValue, changedMap);
                    break;
                case 17:
                    parseItem17(newFieldValue, changedMap);
                    break;
                case 18:
                    parseItem18(newFieldValue, changedMap);
                    break;
                case 19:
                    parseItem19(newFieldValue, changedMap);
                    break;
                case 20:
                    parseItem20(newFieldValue, changedMap);
                    break;
                case 21:
                    parseItem21(newFieldValue, changedMap);
                    break;
                case 22:
                    break;
                default:
                    break;
                }
            }
            else
            {
                map["errors"] = (map["errors"].toStringList() << "Invalid item 22"); // error handling
            }

            if (!fieldCount)
                break;

            ++i;
            current = field.section(splitter, i, i, QString::SectionIncludeLeadingSep);
        }

        map["item22"] = changedMap;
    }

    QString composeItem22(const QVariantMap& binMessage)
    {
        QString message;
        if (binMessage.contains("item22"))
        {
            QVariantMap item22 = binMessage["item22"].toMap();

            QVariantList items = item22["list"].toList();
            QVariantList::ConstIterator it = items.constBegin();
            QVariantList::ConstIterator end = items.constEnd();

            for (; it != end; ++it)
            {
                int itemNo = (*it).toInt();

                if (itemNo)
                {
                    message.append("-");

                    message.append(QString::number(itemNo));

                    message.append("/");
                    switch (itemNo)
                    {
                    case 3:
                        break;
                    case 5:
                        break;
                    case 7:
                        message.append(composeItem7(item22));
                        break;
                    case 8:
                        message.append(composeItem8(item22));
                        break;
                    case 9:
                        message.append(composeItem9(item22));
                        break;
                    case 10:
                        message.append(composeItem10(item22));
                        break;
                    case 13:
                        message.append(composeItem13(item22));
                        break;
                    case 14:
                        message.append(composeItem14(item22));
                        break;
                    case 15:
                        message.append(composeItem15(item22));
                        break;
                    case 16:
                        message.append(composeItem16(item22));
                        break;
                    case 17:
                        message.append(composeItem17(item22));
                        break;
                    case 18:
                        message.append(composeItem18(item22));
                        break;
                    case 19:
                        message.append(composeItem19(item22));
                        break;
                    case 20:
                        message.append(composeItem20(item22));
                        break;
                    case 21:
                        message.append(composeItem21(item22));
                        break;
                    case 22:
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        else
        {
            return QString::null;
        }
        return message;
    }

    void parseArr(QVariantMap& binMessage)
    {
        QStringList fields = binMessage.value("fields").toStringList();
        parseItem3(fields[0], binMessage);
        parseItem7(fields[1], binMessage);
        parseItem13(fields[2], binMessage);
        //parseItem16(fields[3], binMessage);
        parseItem17(fields[3], binMessage);
    }

    QString composeArr(const QVariantMap& binMessage)
    {
        QString result;
        result.append(composeItem3(binMessage));
        result.append("-");
        result.append(composeItem7(binMessage));
        result.append("-");
        result.append(composeItem13(binMessage));
        //result.append("-");
        //result.append(composeItem16(binMessage));
        result.append("-");
        result.append(composeItem17(binMessage));
        return result;
    }

    void parseDep(QVariantMap& binMessage)
    {
        QStringList fields = binMessage.value("fields").toStringList();
        parseItem3(fields[0], binMessage);
        parseItem7(fields[1], binMessage);
        parseItem13(fields[2], binMessage);
        parseItem16(fields[3], binMessage);
        if (fields.count() > 4)
        {
            parseItem18(fields[4], binMessage);
        }
    }

    QString composeDep(const QVariantMap& binMessage)
    {
        QString result;
        result.append(composeItem3(binMessage));
        result.append("-");
        result.append(composeItem7(binMessage));
        result.append("-");
        result.append(composeItem13(binMessage));
        result.append("-");
        result.append(composeItem16(binMessage));

        // fpl2012
        if (binMessage.contains("item18"))
        {
            result.append("-");
            result.append(composeItem18(binMessage));
        }

        return result;
    }

    void parseDla(QVariantMap& binMessage)
    {
        QStringList fields = binMessage.value("fields").toStringList();
        parseItem3(fields[0], binMessage);
        parseItem7(fields[1], binMessage);
        parseItem13(fields[2], binMessage);
        parseItem16(fields[3], binMessage);
        if (fields.count() > 4)
        {
            parseItem18(fields[4], binMessage);
        }
    }

    QString composeDla(const QVariantMap& binMessage)
    {
        QString result;
        result.append(composeItem3(binMessage));
        result.append("-");
        result.append(composeItem7(binMessage));
        result.append("-");
        result.append(composeItem13(binMessage));
        result.append("-");
        result.append(composeItem16(binMessage));

        // fpl2012
        if (binMessage.contains("item18"))
        {
            result.append("-");
            result.append(composeItem18(binMessage));
        }

        return result;
    }

    void parseCnl(QVariantMap& binMessage)
    {
        QStringList fields = binMessage.value("fields").toStringList();
        parseItem3(fields[0], binMessage);
        parseItem7(fields[1], binMessage);
        parseItem13(fields[2], binMessage);
        parseItem16(fields[3], binMessage);
        if (fields.count() > 4)
        {
            parseItem18(fields[4], binMessage);
        }
    }

    QString composeCnl(const QVariantMap& binMessage)
    {
        QString result;
        result.append(composeItem3(binMessage));
        result.append("-");
        result.append(composeItem7(binMessage));
        result.append("-");
        result.append(composeItem13(binMessage));
        result.append("-");
        result.append(composeItem16(binMessage));
        // fpl2012
        if (binMessage.contains("item18"))
        {
            result.append("-");
            result.append(composeItem18(binMessage));
        }

        return result;
    }

    void parseChg(QVariantMap& binMessage)
    {
        QStringList fields = binMessage.value("fields").toStringList();

        parseItem3(fields[0], binMessage);
        parseItem7(fields[1], binMessage);
        parseItem13(fields[2], binMessage);
        parseItem16(fields[3], binMessage);

        int startItemIdx = 4;
        if (!fields[4].left(3).contains(QRegExp("\\d{1,2}/")))
        {
            /// AAAA, it's fpl 2012
            parseItem18(fields[4], binMessage);
            startItemIdx = 5;
        }

        QString field22;
        for (int i = startItemIdx; i < fields.count(); ++i)
        {
            field22 += "-" + fields[i];
        }

        parseItem22(field22, binMessage);
    }

    QString composeChg(const QVariantMap& binMessage)
    {
        QString result;
        result.append(composeItem3(binMessage));
        result.append("-");
        result.append(composeItem7(binMessage));
        result.append("-");
        result.append(composeItem13(binMessage));
        result.append("-");
        result.append(composeItem16(binMessage));

        // fpl2012
        if (binMessage.contains("item18"))
        {
            result.append("-");
            result.append(composeItem18(binMessage));
        }

        result.append(composeItem22(binMessage));

        return result;
    }

    void parseFpl(QVariantMap& binMessage)
    {
        QStringList fields = binMessage.value("fields").toStringList();
        if (fields.count() < 9)
        {
            binMessage["errors"] = QStringList("Too few fields for fpl");
            return;
        }
        parseItem3(fields[0], binMessage);
        parseItem7(fields[1], binMessage);
        parseItem8(fields[2], binMessage);
        parseItem9(fields[3], binMessage);
        parseItem10(fields[4], binMessage);
        parseItem13(fields[5], binMessage);
        parseItem15(fields[6], binMessage);
        parseItem16(fields[7], binMessage);
        parseItem18(fields[8], binMessage);
    }

    QString composeFpl(const QVariantMap& binMessage)
    {
        QString result;
        result.append(composeItem3(binMessage));
        result.append("-");
        result.append(composeItem7(binMessage));
        result.append("-");
        result.append(composeItem8(binMessage));
        result.append("-");
        result.append(composeItem9(binMessage));
        result.append("-");
        result.append(composeItem10(binMessage));
        result.append("-");
        result.append(composeItem13(binMessage));
        result.append("-");
        result.append(composeItem15(binMessage));
        result.append("-");
        result.append(composeItem16(binMessage));
        result.append("-");
        result.append(composeItem18(binMessage));

        return result;
    }

    void parseFields(QVariantMap& binMessage)
    {
        QStringList fields = binMessage.value("fields").toStringList();

        QString signature = fields.value(0).mid(0, 3);

        //ALR | RCF | FPL | CHG | CNL | DLA | DEP | ARR | CPL | EST | CDN | ACP | LAM | RQP | RQS | SPL
        if (signature == "FPL")
        {
            return parseFpl(binMessage);
        }
        else if (signature == "CHG")
        {
            return parseChg(binMessage);
        }
        else if (signature == "CNL")
        {
            return parseCnl(binMessage);
        }
        else if (signature == "DLA")
        {
            return parseDla(binMessage);
        }
        else if (signature == "DEP")
        {
            return parseDep(binMessage);
        }
        else if (signature == "ARR")
        {
            return parseArr(binMessage);
        }
        else
        {
            binMessage["errors"] = QStringList("Unknown flight message type");
        }
    }

    QVariantMap parse(const QString& message)
    {
        QVariantMap result;

        // Split message by delimiter '-'
        int offset = message.startsWith("(") ? 1 : 0;
        int size = message.size() - offset - (message.endsWith(")") ? 1 : 0);
        QString inMessage = message.mid(offset, size);
        inMessage.replace("&", " ");
        QStringList fields = inMessage.split('-');

        result["fields"] = fields;

        parseFields(result);
        return result;
    }

    QString compose(const QVariantMap& binMessage)
    {
        QString message;

        if (binMessage.contains("item3"))
        {
            message.append("(");
            QString signature = binMessage["item3"].toMap()["a"].toString();
            if (signature == "FPL")
            {
                message.append(composeFpl(binMessage));
            }
            else if (signature == "CHG")
            {
                message.append(composeChg(binMessage));
            }
            else if (signature == "CNL")
            {
                message.append(composeCnl(binMessage));
            }
            else if (signature == "DLA")
            {
                message.append(composeDla(binMessage));
            }
            else if (signature == "DEP")
            {
                message.append(composeDep(binMessage));
            }
            else if (signature == "ARR")
            {
                message.append(composeArr(binMessage));
            }
            message.append(")");
        }

        return message;
    }

    bool isPresent(const QVariantMap& binMessage)
    {
        bool result = false;
        //Поле 10a содержит «E», «J», «M», «P» или «Q»
        if (binMessage["item10"].toMap()["a"].toString().contains(QRegExp("(E[^\\d]|J[^\\d]|M[^\\d]|P[^\\d]|Q)")))
        {
            result = true;
        }
        //Поле 10b содержит «D» или «N»
        if (binMessage["item10"].toMap()["a"].toString().contains(QRegExp("(D|N)")))
        {
            result = true;
        }
        // Элемент типа 18 «PER/»
        if (binMessage["item18"].toMap().contains("PER"))
        {
            result = binMessage["item18"].toMap()["PER"].toString().length() > 1;
        }
        return result;
    }

    bool isNew(const QVariantMap& binMessage)
    {
        bool result = false;
        // Поле 10a содержит цифровой знак
        if (binMessage["item10"].toMap()["a"].toString().contains(QRegExp("\\d")))
        {
            result = true;
        }

        // Поле 10b содержит:
        //  “E”, “H” or “L”
        //  Цифровой знак, или
        //  Более чем два символа
        if (binMessage["item10"].toMap()["b"].toString().contains(QRegExp("(E|H|L|\\d)")))
        {
            result = true;
        }
        if (binMessage["item10"].toMap()["b"].toString().length() > 1)
        {
            result = true;
        }

        if (binMessage["item3"].toMap()["a"] == "CNL")
        {
            result = binMessage.contains("item18");
        }
        if (binMessage["item3"].toMap()["a"] == "CHG")
        {
            result = binMessage.contains("item18");
        }
        if (binMessage["item3"].toMap()["a"] == "DEP")
        {
            result = binMessage.contains("item18");
        }
        if (binMessage["item3"].toMap()["a"] == "DLA")
        {
            result = binMessage.contains("item18");
        }

        return result;
    }

    FplStandard whatFplStadnard(const QVariantMap& binMessage)
    {
        bool present = isPresent(binMessage);
        bool new_ = isNew(binMessage);

        if (present && new_)
            return INVALID;

        if (present)
            return PRESENT;

        if (new_)
            return NEW;

        // headshot
        return INVALID;
    }

    QVariantMap convertFromNew2012ToPresent(const QVariantMap& newBinMessage)
    {
        QVariantMap presentFpl = newBinMessage;

        if (presentFpl.contains("item10"))
        {
            QVariantMap item10 = presentFpl["item10"].toMap();
            QVariantMap item18 = presentFpl["item18"].toMap();
            QString item10a = item10["a"].toString();
            QString item10b = item10["b"].toString();
            QString item18nav = item18["NAV"].toString();
            QString item18com = item18["COM"].toString();
            QString item18dat = item18["DAT"].toString();
            QString item18rmk = item18["RMK"].toString();
            QString item18pbn = item18["PBN"].toString();
            QString item18sts = item18["STS"].toString();

            // DAT/     COM/
            item18com.append(" " + item18dat);
            item18dat.clear();

            if (!item10a.isEmpty())
            {
                //S     VOL
                item10a = item10a.replace("S", "VOL");

                //SF    S
                //item10a = item10a.replace("VOLF", "S");

                //A     Z   NAV/GBAS
                item10a = item10a.replace("A", "Z");
                item18nav = item18nav.append(" GBAS");

                //B     Z   NAV/LPV
                item10a = item10a.replace("B", "Z");
                item18nav = item18nav.append(" LPV");

                // E1       Z   COM/FMC WPR ACARS
                item10a = item10a.replace("E1", "Z");
                item18com = item18com.append(" FMC WPR ACARS");

                //E2        Z   COM/DFIS ACARS
                item10a = item10a.replace("E2", "Z");
                item18com = item18com.append(" DFIS ACARS");

                //E3        Z   COM/PDC ACARS
                item10a = item10a.replace("E3", "Z");
                item18com = item18com.append(" PDC ACARS");

                //J1        J   DAT/V
                item10a = item10a.replace("J1", "J");
                item18dat = item18dat.append("V");

                //J2       J   DAT/H
                item10a = item10a.replace("J2", "J");
                item18dat = item18dat.append("H");

                // J3      J   DAT/V
                item10a = item10a.replace("J3", "J");
                item18dat = item18dat.append("V");

                // J4      J   DAT/V
                item10a = item10a.replace("J4", "J");
                item18dat = item18dat.append("V");

                // J5      J   DAT/S
                item10a = item10a.replace("J5", "J");
                item18dat = item18dat.append("S");
                /*
                 * J6      J   DAT/S
                 */
                item10a = item10a.replace("J6", "J");
                item18dat = item18dat.append("S");
                /*
                 * J7      J   DAT/S
                 */
                item10a = item10a.replace("J7", "J");
                item18dat = item18dat.append("S");

                /*
                 * M1       Z   COM/INMARSAT
                 */
                item10a = item10a.replace("M1", "Z");
                item18com = item18com.append(" INMARSAT");
                /*
                 * M2      Z   COM/MTSAT
                 */
                item10a = item10a.replace("M2", "Z");
                item18com = item18com.append(" MTSAT");
                /*
                 * M3      Z   COM/IRIDIUM
                 */
                item10a = item10a.replace("M3", "Z");
                item18com = item18com.append(" IRIDIUM");

                /*
                 * R    PBN/A1  RZ  NAV/RNAV10 RNP10
                 */
                item10a = item10a.replace("R", "RZ");

                if (!item18pbn.isEmpty())
                {
                    //R PBN/A1  RZ  NAV/RNAV10 RNP10
                    if (item18pbn.contains("A1"))
                    {
                        item18nav = item18nav.append(" RNAV10 RNP10");
                    }
                    //R PBN/B1  RZ  NAV/RNAV5
                    if (item18pbn.contains(QRegExp("B\\d")))
                    {
                        item18nav = item18nav.append(" RNAV5");
                    }

                    // R    PBN/C1  RZ  NAV/RNAV2
                    if (item18pbn.contains(QRegExp("C\\d")))
                    {
                        item18nav = item18nav.append(" RNAV2");
                    }
                    // R    PBN/D1  PRZ NAV/RNAV1
                    if (item18pbn.contains(QRegExp("(D\\d|O\\d)")))
                    {
                        item10a = item10a.append("P");
                        item18nav = item18nav.append(" RNAV1");
                    }

                    //R PBN/L1  RZ  NAV/RNP4
                    if (item18pbn.contains("L1"))
                    {
                        item18nav = item18nav.append(" RNP4");
                    }

                    //R PBN/S1  RZ  NAV/RNP APRCH
                    if (item18pbn.contains("S1"))
                    {
                        item18nav = item18nav.append(" RNP APRCH");
                    }

                    //R PBN/S2  RZ  NAV/RNP APRCH BARO VNAV
                    if (item18pbn.contains("S2"))
                    {
                        item18nav = item18nav.append(" RNP APRCH BARO VNAV");
                    }

                    //R PBN/T1  RZ  NAV/RNP AR APRCH RF
                    if (item18pbn.contains("T1"))
                    {
                        item18nav = item18nav.append(" RNP AR APRCH RF");
                    }

                    //R PBN/T2  RZ  NAV/RNP AR APRCH
                    if (item18pbn.contains("T2"))
                    {
                        item18nav = item18nav.append(" RNP AR APRCH");
                    }
                }

            }
            if (!item10b.isEmpty())
            {
                //E     SD
                item10b.replace("E", "SD");

                //H     S
                item10b.replace("H", "S");

                //L     SD
                item10b.replace("L", "SD");

                // B1      D
                // etc
                item10b.replace(QRegExp("B\\d"), "D");
                item10b.replace(QRegExp("U\\d"), "D");
                item10b.replace(QRegExp("V\\d"), "D");

                //D1        D   RMK/ ADSC
                //G1      D   RMK/ ADSC
                if (item10b.contains(QRegExp("(D1|G1)")))
                {
                    item18rmk.append("ADSC");
                }
                item10b.replace(QRegExp("D\\d"), "D");
                item10b.replace(QRegExp("G\\d"), "D");

            }

            //STS/                    STS/ < text after STS/ >
            //STS/ATFMX                 STS/ATFMXEXEMPTAPPROVED
            if (item18sts.contains("ATFMX"))
                item18sts.replace("ATFMX", "ATFMXEXEMPTAPPROVED");

            if (item18.contains("SUR"))
            {
                item18rmk.append(" SUR " + item18["SUR"].toString());
            }

            if (item18.contains("DLE"))
            {
                item18rmk.append(" DLE " + item18["SUR"].toString());
            }

            if (item18.contains("ORGN"))
            {
                item18rmk.append(" ORGN " + item18["SUR"].toString());
            }

            if (item18.contains("TALT"))
            {
                item18rmk.append(" TALT " + item18["SUR"].toString());
            }

            item18.remove("SUR");
            item18.remove("DLE");
            item18.remove("ORGN");
            item18.remove("TALT");
            item18.remove("PBN");

            if (item18dat.contains("S"))
            {
                item18dat.remove("S");
                item18dat.append("S");
            }

            if (item18dat.contains("V"))
            {
                item18dat.remove("V");
                item18dat.append("V");
            }

            if (item10a.contains("Z"))
            {
                // REMOVE ALL
                item10a.remove("Z");
                // APPEND ONE
                item10a.append("Z");
            }

            if (item10a.contains("J"))
            {
                // REMOVE ALL
                item10a.remove("J");
                // APPEND ONE
                item10a.append("J");
            }

            if (item10a.contains("R"))
            {
                // REMOVE ALL
                item10a.remove("R");
                // APPEND ONE
                item10a.append("R");
            }

            if (item10a.contains("P"))
            {
                // REMOVE ALL
                item10a.remove("P");
                // APPEND ONE
                item10a.append("P");
            }

            // Remove P1-P9
            item10a.remove(QRegExp("P\\d"));

            // Remove multiply SD
            if (item10b.contains("S"))
            {
                item10b.remove("S");
                item10b.append("S");
            }

            if (item10b.contains("D"))
            {
                item10b.remove("D");
                item10b.append("D");
            }

            item10["a"] = item10a;
            item10["b"] = item10b;

            item18["NAV"] = item18nav;
            item18["COM"] = item18com;
            item18["DAT"] = item18dat;
            item18["RMK"] = item18rmk;
            item18["STS"] = item18sts;

            presentFpl["item10"] = item10;
            presentFpl["item18"] = item18;
        }

        if (presentFpl["item3"].toMap()["a"] == "CNL")
        {
            presentFpl.remove("item18");
        }
        if (presentFpl["item3"].toMap()["a"] == "CHG")
        {
            presentFpl.remove("item18");
        }
        if (presentFpl["item3"].toMap()["a"] == "DEP")
        {
            presentFpl.remove("item18");
        }
        if (presentFpl["item3"].toMap()["a"] == "DLA")
        {
            presentFpl.remove("item18");
        }

        if (presentFpl.contains("item22"))
        {
            QVariantMap item22 = presentFpl["item22"].toMap();
            presentFpl["item22"] = convertFromNew2012ToPresent(item22);
        }

        return presentFpl;
    }
}
