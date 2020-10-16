#include "MetarSpeciParser.h"

MetarSpeci *MetarSpeciParser::doit(const QString &rawdata)
{
    begError = endError = 0;
    curIndex = -1;
    result.reset(new MetarSpeci());
    result->arrTime = QDateTime::currentDateTimeUtc();
    try
    {
        QRegExp startRegExp("\\bMETAR\\b|\\bSPECI\\b");
        int startIndex = startRegExp.indexIn(rawdata);
        if (-1 == startIndex)
            throw MetarSpeci::NotMetarSpeci;

        int endIndex = rawdata.indexOf("=", startIndex);
        if (-1 == endIndex)
            throw MetarSpeci::NotMetarSpeci;

        data = rawdata.mid(startIndex, endIndex - startIndex).split(QRegExp("\\s+"), QString::SkipEmptyParts);

        result->source = data.join(" ");

        curIndex = 0;

        parseType();
        parseCor();
        parseAirport();
        parseTime();
        parseNil();
        parseAuto();
        parseWind();
        parseCavok();
        if (!result->isCavok)
        {
            parseVisibility();
            parseRunwayVisibility();
            parseWeatherEvents();
            parseCloudiness();
        }
        parseTemperature();
        parsePressure();
        parseRecentWeather();
        parseWindShear();
        parseSeaState();
        parseRunwayState();
        parseThend();
        parseRmk();

        if (curIndex != data.size())
        {
            lastError = MetarSpeci::AdditionalUnknownFields;
            setErrorPos();
            endError = result->source.size();
        }
        else
            lastError = MetarSpeci::Ok;
    }
    catch (const MetarSpeci::Error &error)
    {
        lastError = error;
        setErrorPos();
    }
    return result.take();
}

void MetarSpeciParser::parseType()
{
    if (data.size() <= curIndex)
        throw MetarSpeci::NotMetarSpeci;

    if ("METAR" == data.at(curIndex))
        result->type = MetarSpeci::Metar;
    else if ("SPECI" == data.at(curIndex))
        result->type = MetarSpeci::Speci;
    else
        throw MetarSpeci::NotMetarSpeci;

    ++curIndex;
}

void MetarSpeciParser::parseCor()
{
    if (data.size() > curIndex && "COR" == data.at(curIndex))
    {
        ++curIndex;
        result->isCor = true;
    }
    else
        result->isCor = false;
}

void MetarSpeciParser::parseAirport()
{
    if (data.size() <= curIndex)
        throw MetarSpeci::InvalidAirport;

    QRegExp airportRegExp("^([A-Z]{4})$");
    if (!airportRegExp.exactMatch(data.at(curIndex)))
        throw MetarSpeci::InvalidAirport;

    result->airport = airportRegExp.cap(1);
    ++curIndex;
}

void MetarSpeciParser::parseTime()
{
    if (data.size() <= curIndex)
        throw MetarSpeci::InvalidTime;

    QRegExp timeRegExp("^(\\d{2})?(\\d{2})(\\d{2})Z$");
    if (!timeRegExp.exactMatch(data.at(curIndex)))
        throw MetarSpeci::InvalidTime;

    QDateTime now = QDateTime::currentDateTimeUtc();
    QTime metarTime(timeRegExp.cap(2).toInt(), timeRegExp.cap(3).toInt());
    if (!metarTime.isValid())
        throw MetarSpeci::InvalidTime;

    QDateTime tomorrow;
    QDateTime today;
    QDateTime yesterday;

    QString dayString = timeRegExp.cap(1);
    if (!dayString.isEmpty())
    {
        int year, month, day;
        now.date().getDate(&year, &month, &day);
        day = dayString.toInt();
        today = QDateTime(QDate(year, month, day), metarTime);
        tomorrow = today.addMonths(1);
        yesterday = today.addMonths(-1);
    }
    else
    {
        today = QDateTime(now.date().addDays(1), metarTime);
        tomorrow = today.addDays(1);
        yesterday = today.addDays(-1);
    }

    qint64 tomorrowTime = qAbs(tomorrow.msecsTo(now));
    qint64 todayTime = qAbs(today.msecsTo(now));
    qint64 yesterdayTime = qAbs(yesterday.msecsTo(now));

    if (tomorrowTime <= todayTime && tomorrowTime <= yesterdayTime)
        result->date = tomorrow;
    else if (yesterdayTime <= tomorrowTime && yesterdayTime <= todayTime)
        result->date = yesterday;
    else
        result->date = today;

    if (!result->date.isValid())
        throw MetarSpeci::InvalidTime;

    ++curIndex;
}

void MetarSpeciParser::parseNil()
{
    if (data.size() > curIndex && "NIL" == data.at(curIndex))
    {
        ++curIndex;
        result->isNil = true;
    }
    else
        result->isNil = false;
}

void MetarSpeciParser::parseAuto()
{
    if (data.size() > curIndex && "AUTO" == data.at(curIndex))
    {
        ++curIndex;
        result->isAuto = true;
    }
    else
        result->isAuto = false;
}

void MetarSpeciParser::parseWind()
{
    if (data.size() <= curIndex)
        throw MetarSpeci::InvalidWind;

    QRegExp windRegExp("^(\\d{3}|VRB)(\\d{2})(G(P)?(\\d{2}))?(MPS|KT|KMH)$");
    if (!windRegExp.exactMatch(data.at(curIndex)))
        throw MetarSpeci::InvalidWind;

    const QString direction = windRegExp.cap(1);
    if ("VRB" != direction)
    {
        result->wind.isVariable = false;
        result->wind.direction = direction.toInt();
    }
    else
        result->wind.isVariable = true;

    result->wind.averageSpeed = windRegExp.cap(2).toInt();

    if (!windRegExp.cap(3).isEmpty())
    {
        result->wind.haveGust = true;
        result->wind.isAbove = !windRegExp.cap(4).isEmpty();
        result->wind.gust = windRegExp.cap(5).toInt();
    }
    else
        result->wind.haveGust = false;

    if (!result->wind.isVariable && 0 == result->wind.direction && 0 != result->wind.averageSpeed)
        throw MetarSpeci::InvalidWind;

    result->wind.speedType = windRegExp.cap(6);

    ++curIndex;

    QRegExp windExRegExp("^(\\d{3})V(\\d{3})$");
    if (!result->wind.isVariable && data.size() > curIndex && windExRegExp.exactMatch(data.at(curIndex)))
    {
        result->wind.haveVariations = true;
        result->wind.variationFrom = windExRegExp.cap(1).toInt();
        result->wind.variationTo = windExRegExp.cap(2).toInt();
        ++curIndex;
    }
    else
        result->wind.haveVariations = false;
}

void MetarSpeciParser::parseCavok()
{
    if (data.size() > curIndex && "CAVOK" == data.at(curIndex))
    {
        ++curIndex;
        result->isCavok = true;
    }
    else
        result->isCavok = false;
}

void MetarSpeciParser::parseVisibility()
{
    if (data.size() <= curIndex)
        throw MetarSpeci::InvalidVisibility;

    QRegExp visibilityRegExp("^(\\d{4})(N|NE|E|SE|S|SW|W|NW|NDV)?$");
    if (!visibilityRegExp.exactMatch(data.at(curIndex)))
        throw MetarSpeci::InvalidVisibility;

    result->visibilityMin = visibilityRegExp.cap(1).toInt();
    result->visibilityMinDirection = visibilityRegExp.cap(2);
    result->haveMaxVisibility = false;

    ++curIndex;

    if (data.size() > curIndex && visibilityRegExp.exactMatch(data.at(curIndex)))
    {
        result->haveMaxVisibility = true;
        result->visibilityMax = visibilityRegExp.cap(1).toInt();
        result->visibilityMaxDirection = visibilityRegExp.cap(2);
        ++curIndex;
    }
}

void MetarSpeciParser::parseRunwayVisibility()
{
    QRegExp runwayVisibilityRangeRegExp("^R(\\d{2})(LL|L|C|R|RR)?/([MP])?(\\d{4})(V([MP])?(\\d{4}))?([NDU])?$");
    while (data.size() > curIndex && runwayVisibilityRangeRegExp.exactMatch(data.at(curIndex)))
    {
        MetarSpeci::RunwayVisibilityRange rvr;
        rvr.runwayId = runwayVisibilityRangeRegExp.cap(1).toInt();
        rvr.runwayPlace = runwayVisibilityRangeRegExp.cap(2);
        rvr.outOfRange = runwayVisibilityRangeRegExp.cap(3);
        rvr.visibility = runwayVisibilityRangeRegExp.cap(4).toInt();
        if (!runwayVisibilityRangeRegExp.cap(5).isEmpty())
        {
            rvr.isVariable = true;
            rvr.outOfRangeMax = runwayVisibilityRangeRegExp.cap(6);
            rvr.visibilityMax = runwayVisibilityRangeRegExp.cap(7).toInt();
        }
        else
            rvr.isVariable = false;
        rvr.tendency = runwayVisibilityRangeRegExp.cap(8);
        result->rvr.append(rvr);
        ++curIndex;
    }
}

// (DZ|UP|RA|SN|SG|PL|DS|SS|FZDZ|FZRA|FZUP|SHGR|SHGS|SHRA|SHSN|SHPL|TSGR|TSGS|TSPL|TSRA|TSSN|IC|FG|BR|SA|DU|HZ|FU|VA|SQ|PO|FC|TS|BCFG|BLDU|BLSA|BLSN|DRDU|DRSA|DRSN|FZFG|MIFG|PRFG|VCTSRA|VCBLDU)
void MetarSpeciParser::parseWeatherEvents()
{
    QRegExp weatherEventRangeRegExp("^(\\+|-|VC)?((MI|BC|PR|DR|BL|SH|TS|FZ)|(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+|(MI|BC|PR|DR|BL|SH|TS|FZ)(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+)$");
    while (data.size() > curIndex && weatherEventRangeRegExp.exactMatch(data.at(curIndex)))
    {
        MetarSpeci::WeatherEvent weatherEvent;
        weatherEvent.strength = weatherEventRangeRegExp.cap(1);
        weatherEvent.type = weatherEventRangeRegExp.cap(2);
        result->weatherEvents.append(weatherEvent);
        ++curIndex;
    }
}

void MetarSpeciParser::parseCloudiness()
{
    if (data.size() <= curIndex)
        return;

    QRegExp cloudinessRegExp("^(FEW|SCT|BKN|OVC)(\\d{3})(CB|TCU|///)?$");
    QRegExp verticalVisibilityRegExp("^VV(\\d{3}|///)$");
    if ("SKC" == data.at(curIndex))
    {
        result->isCloudinessAndVerticalVisibilityAvailable = true;
        result->cloudinessAndVerticalVisibility.type = MetarSpeci::CloudinessAndVerticalVisibility::Skc;
        ++curIndex;
    }
    else if ("NSC" == data.at(curIndex))
    {
        result->isCloudinessAndVerticalVisibilityAvailable = true;
        result->cloudinessAndVerticalVisibility.type = MetarSpeci::CloudinessAndVerticalVisibility::Nsc;
        ++curIndex;
    }
    else if (verticalVisibilityRegExp.exactMatch(data.at(curIndex)))
    {
        result->isCloudinessAndVerticalVisibilityAvailable = true;
        result->cloudinessAndVerticalVisibility.type = MetarSpeci::CloudinessAndVerticalVisibility::Visibility;
        result->cloudinessAndVerticalVisibility.verticalVisibility = cloudinessRegExp.cap(1);
        ++curIndex;
    }
    else if (cloudinessRegExp.exactMatch(data.at(curIndex)))
    {
        result->isCloudinessAndVerticalVisibilityAvailable = true;
        result->cloudinessAndVerticalVisibility.type = MetarSpeci::CloudinessAndVerticalVisibility::Clouds;
        do
        {
            MetarSpeci::CloudinessAndVerticalVisibility::Cloudiness cloudiness;
            cloudiness.cloudAmout = cloudinessRegExp.cap(1);
            cloudiness.height = cloudinessRegExp.cap(2).toInt();
            cloudiness.cloudType = cloudinessRegExp.cap(3);
            result->cloudinessAndVerticalVisibility.clouds.append(cloudiness);
            ++curIndex;
        } while(cloudinessRegExp.exactMatch(data.at(curIndex)));
    }
}

void MetarSpeciParser::parseTemperature()
{
    if (data.size() <= curIndex)
        throw MetarSpeci::InvalidTemperature;

    QRegExp temperatureRegExp("^((M)?(\\d{2})/(M)?(\\d{2}))$");
    if (!temperatureRegExp.exactMatch(data.at(curIndex)))
        throw MetarSpeci::InvalidTemperature;

    result->temperature = temperatureRegExp.cap(3).toInt();
    if (!temperatureRegExp.cap(2).isEmpty())
        result->temperature *= -1;

    result->dewPoint = temperatureRegExp.cap(5).toInt();
    if (!temperatureRegExp.cap(4).isEmpty())
        result->dewPoint *= -1;

    ++curIndex;
}

void MetarSpeciParser::parsePressure()
{
    QRegExp pressureRegExp("^([QA])(\\d{4})$");
    if (data.size() <= curIndex || !pressureRegExp.exactMatch(data.at(curIndex)))
        throw MetarSpeci::InvalidPressure;

    result->pressureType = pressureRegExp.cap(1);
    result->pressure = pressureRegExp.cap(2).toInt();
    ++curIndex;
}

// (REFZDZ|REFZRA|REFZDZRA|REFZRADZ|REDZ|RERA|RETS| RESN|RESG|REGR|REGS|REPL|RESHRA|RESHSN|RESHSG|RESHGR|RESHGS|RESHPL|REIC|REBLSN|RESS|REDS|REFC|REVA)
void MetarSpeciParser::parseRecentWeather()
{
    QRegExp recentWeatherRegExp("^RE((MI|BC|PR|DR|BL|SH|TS|FZ)|(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+|(MI|BC|PR|DR|BL|SH|TS|FZ)(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+)$");
    while (data.size() > curIndex && recentWeatherRegExp.exactMatch(data.at(curIndex)))
    {
        result->recentWeather.append(recentWeatherRegExp.cap(1));
        ++curIndex;
    }
}

void MetarSpeciParser::parseWindShear()
{
    while (data.size() > curIndex && "WS" == data.at(curIndex))
    {
        MetarSpeci::WindShear windShift;
        if (data.size() > curIndex + 2 && "ALL" == data.at(curIndex + 1) && "RWY" == data.at(curIndex + 2))
        {
            windShift.isAll = true;
            curIndex += 3;
        }
        else
        {
            windShift.isAll = false;
            QRegExp windShiftRegExp("^R(\\d{2})(LL|L|C|R|RR)?$");
            if (data.size() > curIndex + 1 && windShiftRegExp.exactMatch(data.at(curIndex + 1)))
            {
                windShift.runwayId = windShiftRegExp.cap(1).toInt();
                windShift.runwayPlace = windShiftRegExp.cap(2);
                curIndex += 2;
            }
            else
                return;
        }
        result->windShear.append(windShift);
    }
}

void MetarSpeciParser::parseSeaState()
{
    QRegExp seaStateRegExp("^W(\\d{2})/S(\\d)$");
    if (data.size() > curIndex && seaStateRegExp.exactMatch(data.at(curIndex)))
    {
        result->isSeaStateAvailable = true;
        result->seaTemperature = seaStateRegExp.cap(1).toInt();
        result->seaState = seaStateRegExp.cap(2).toInt();
        ++curIndex;
    }
    else
        result->isSeaStateAvailable = false;
}

void MetarSpeciParser::parseRunwayState()
{
    //                            1------1  2-----3=====34=====45======526---------6
    QRegExp runwayStateRegExp("^R?(\\d{2})/?(CLRD|(\\d|/)(\\d|/)(\\d{2}))(\\d{2}|//)$");
    while (data.size() > curIndex && runwayStateRegExp.exactMatch(data.at(curIndex)))
    {
        MetarSpeci::RunwayState runwayState;
        runwayState.runwayId = runwayStateRegExp.cap(1).toInt();

        if ("CLRD" == runwayStateRegExp.cap(2))
        {
            runwayState.isCLRD = true;
        }
        else
        {
            runwayState.isCLRD = false;
            QString coveringConditions = runwayStateRegExp.cap(3);
            if ("/" != coveringConditions)
            {
                runwayState.isCoveringConditionsSet = true;
                runwayState.coveringConditions = coveringConditions.toInt();
            }
            else
                runwayState.isCoveringConditionsSet = false;

            QString coveringRange = runwayStateRegExp.cap(4);
            if ("/" != coveringRange)
            {
                runwayState.isCoveringRangeSet = true;
                runwayState.coveringRange = coveringRange.toInt();
            }
            else
                runwayState.isCoveringRangeSet = false;

            runwayState.coveringThick = runwayStateRegExp.cap(5).toInt();

            QString cohesion = runwayStateRegExp.cap(6);
            if ("/" != cohesion)
            {
                runwayState.isCohesionSet = true;
                runwayState.cohesion = cohesion.toInt();
            }
            else
                runwayState.isCohesionSet = false;
        }
        result->runwayState.append(runwayState);
        ++curIndex;
    }
}

void MetarSpeciParser::parseThend()
{
    if (data.size() <= curIndex)
        return;

    result->isThendAvailable = false;
    result->thend.isWindAvailable = false;
    result->thend.isVisibilityAvailable = false;
    result->thend.isNsw = false;
    result->thend.cloudlessType = MetarSpeci::Thend::CloudlessUnAvailable;

    if ("NOSIG" == data.at(curIndex))
    {
        result->isThendAvailable = true;
        result->thend.type = data.at(curIndex++);
    }
    else if ("TEMPO" == data.at(curIndex) || "BECMG" == data.at(curIndex))
    {
        result->isThendAvailable = true;
        result->thend.type = data.at(curIndex++);

        if (result->thend.type == "TEMPO")
        {
            QRegExp thendTimeRegExp("^(FM|TL)(\\d{2})(\\d{2})$");
            if (data.size() > curIndex && thendTimeRegExp.exactMatch(data.at(curIndex)))
            {
                MetarSpeci::Thend::Time time;
                time.type = thendTimeRegExp.cap(1);
                time.hours = thendTimeRegExp.cap(2).toInt();
                time.minits = thendTimeRegExp.cap(3).toInt();
                result->thend.time.append(time);
                ++curIndex;
                if (time.type == "FM")
                {
                    QRegExp thendTillTimeRegExp("^(TL)(\\d{2})(\\d{2})$");
                    if (data.size() > curIndex && thendTillTimeRegExp.exactMatch(data.at(curIndex)))
                    {
                        time.type = thendTimeRegExp.cap(1);
                        time.hours = thendTimeRegExp.cap(2).toInt();
                        time.minits = thendTimeRegExp.cap(3).toInt();
                        result->thend.time.append(time);
                        ++curIndex;
                    }
                }
            }
        }
        else
        {
            QRegExp thendTimeRegExp("^(AT|FM|TL)(\\d{2})(\\d{2})$");
            if (data.size() > curIndex && thendTimeRegExp.exactMatch(data.at(curIndex)))
            {
                MetarSpeci::Thend::Time time;
                time.type = thendTimeRegExp.cap(1);
                time.hours = thendTimeRegExp.cap(2).toInt();
                time.minits = thendTimeRegExp.cap(3).toInt();
                result->thend.time.append(time);
                ++curIndex;
                if (time.type == "FM")
                {
                    QRegExp thendTillTimeRegExp("^(TL)(\\d{2})(\\d{2})$");
                    if (data.size() > curIndex && thendTillTimeRegExp.exactMatch(data.at(curIndex)))
                    {
                        time.type = thendTimeRegExp.cap(1);
                        time.hours = thendTimeRegExp.cap(2).toInt();
                        time.minits = thendTimeRegExp.cap(3).toInt();
                        result->thend.time.append(time);
                        ++curIndex;
                    }
                }
            }
        }

        QRegExp thendWindRegExp("^(\\d{3}|VRB)(\\d{2})(G(P)?(\\d{2}))?(MPS|KT|KMH)$");
        if (data.size() > curIndex && thendWindRegExp.exactMatch(data.at(curIndex)))
        {
            result->thend.isWindAvailable = true;
            QString windDirection = thendWindRegExp.cap(1);
            if ("VRB" != windDirection)
            {
                result->thend.wind.isVariable = false;
                result->thend.wind.direction = windDirection.toInt();
            }
            else
                result->thend.wind.isVariable = true;
            result->thend.wind.averageSpeed = thendWindRegExp.cap(2).toInt();
            if (!thendWindRegExp.cap(3).isEmpty())
            {
                result->thend.wind.haveGust = true;
                result->thend.wind.isGustAbove = !thendWindRegExp.cap(4).isEmpty();
                result->thend.wind.gust = thendWindRegExp.cap(5).toInt();
            }
            else
                result->thend.wind.haveGust = false;
            result->thend.wind.speedType = thendWindRegExp.cap(5);
            ++curIndex;
        }

        if (data.size() > curIndex && "CAVOK" == data.at(curIndex))
        {
            ++curIndex;
            result->thend.isCavok = true;
        }
        else
        {
            QRegExp thendVisibilityRegExp("^(\\d{4})$");
            if (data.size() > curIndex && thendVisibilityRegExp.exactMatch(data.at(curIndex)))
            {
                result->thend.isVisibilityAvailable = true;
                result->thend.visibility = thendVisibilityRegExp.cap(1).toInt();
                ++curIndex;
            }

            QRegExp thendWeatheEventRegExp("^(NSW|((MI|BC|PR|DR|BL|SH|TS|FZ)|(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+|(MI|BC|PR|DR|BL|SH|TS|FZ)(DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PO|SQ|FC|SS|DS)+))$");
            while (data.size() > curIndex && thendWeatheEventRegExp.exactMatch(data.at(curIndex)))
            {
                QString ww = thendWeatheEventRegExp.cap(1);
                if ("NSW" == ww)
                {
                    result->thend.isNsw = true;
                    ++curIndex;
                    break;
                }
                else
                    result->thend.weatherEvents.append(ww);
                ++curIndex;
            }

            if (data.size() <= curIndex)
                return;

            QRegExp thendCloudinessRegExp("^(FEW|SCT|BKN|OVC)(\\d{3})(CB|TCU|///)?$");
            QRegExp thendVerticalVisibilityRegExp("^VV(\\d{3}|///)$");
            if ("NSC" == data.at(curIndex))
            {
                result->thend.cloudlessType = MetarSpeci::Thend::Nsc;
                ++curIndex;
            }
            else if (thendCloudinessRegExp.exactMatch(data.at(curIndex)))
            {
                result->thend.cloudlessType = MetarSpeci::Thend::Clouds;
                result->thend.cloudAmout = thendCloudinessRegExp.cap(1);
                result->thend.cloudHeight = thendCloudinessRegExp.cap(2).toInt();
                result->thend.cloudType = thendCloudinessRegExp.cap(3);
                ++curIndex;
            }
            else if (thendVerticalVisibilityRegExp.exactMatch(data.at(curIndex)))
            {
                result->thend.cloudlessType = MetarSpeci::Thend::Visibility;
                result->thend.visibilityHeight = thendVerticalVisibilityRegExp.cap(1).toInt();
                ++curIndex;
            }
        }
    }
}

void MetarSpeciParser::parseRmk()
{
    QRegExp qfeRegExp("^QFE(\\d{3})$");
    QRegExp qbbRegExp("^QBB(\\d{3})$");

    if (data.size() > curIndex && "RMK" == data.at(curIndex))
    {
        if (qfeRegExp.exactMatch(data.at(curIndex)))
            result->qfe = qfeRegExp.cap(1);
        else if (qbbRegExp.exactMatch(data.at(curIndex)))
            result->qbb = qfeRegExp.cap(1);

        result->rmk = data.mid(curIndex);
        curIndex = data.size();
    }
}

void MetarSpeciParser::setErrorPos()
{
    if (curIndex >= data.size() || curIndex < 0)
    {
        begError = 0;
        endError = result->source.size();
    }
    else
    {
        begError = 0;
        for (int i = 0; i < curIndex; ++i)
            begError += data.at(i).size() + 1;
        endError = begError + data.at(curIndex).size();
    }
}
