#ifndef LIC_H
#define LIC_H

#include <QString>

//
// ZAO "Peleng"
//
// Generate and check license class
//  - Get SATA HDD serial number
//  - Generate license file
//  - Check license
//

const QString test_str("Peleng software 2012");

class lic
{
    static QString chd();
public:
    lic();
    static bool check_license(QString lic_file);
    static bool generate_license(QString lic_file, QString serial_no);
};

#endif // LIC_H
