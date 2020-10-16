#ifndef LISTCONSTRUCTOR_H
#define LISTCONSTRUCTOR_H

#include "EditHeaderInterface.h"

class CListConstructor:
        public EditHeaderInterface
{
public:
    CListConstructor();
    CListConstructor(const QString &ObjectName);

    void setObjectName(QString &s){
        sObjectName = s;
    }

    void loadItems();

    virtual QStringList languages() const;
    virtual EditHeaderItems items() const;
    virtual void setItems(const EditHeaderItems & Items);

private:
    EditHeaderItems     m_Items;
    QString             sObjectName;
};

#endif // LISTCONSTRUCTOR_H
