#include "CmdLineArguments.h"

#include <QtCore/QtGlobal>

#if defined(_MSC_VER)
#include "xgetopt.h"
#else
#include <getopt.h>
#endif

#include <QDebug>

#include <QSettings>

const QString sysKeyOfStartApplication = "KeyOfStartApplication";

// Adds or replaces value of the option in argument list.
static void addArgument(QStringList &args, const QString &option, const QString &value = QString())
{
    // Nothing to add or replace.
    if ( value.isEmpty() )
        return;

    // Find first option in the argument list and replace value for it.
    int i = 0;
    for ( ; i < args.size(); i++ )
    {
        if ( option == args[i] )
        {
            // Return string associated with an option.
            if ( i+1 < args.size() )
                args[i+1] = value;
            break;
        }
    }

    // Option is not found - add it to the end of the argument list.
    if ( i == args.size() )
        args << option << value;
}

// Parse command line using getopts function (UNIX style options)
static QStringList parseCommandLine(int argc, char** argv)
{
    QStringList arguments;

    int c;
    QString commandLine("Command line: ");

    QSettings settings;
    bool haveX = false;

    while ( true )
    {
        int option_index = 0;
        static struct option long_options[] = {
            {"e",               optional_argument, 0,  'e' },
            {"config",          required_argument, 0,  'c' },
            {"log",             required_argument, 0,  'l' },
            {"user",            required_argument, 0,  'u' },
            {"password",        required_argument, 0,  'p' },
            {"xml",             optional_argument, 0,  'x' },
            {"host",            required_argument, 0,  0 },
            {"port",            required_argument, 0,  0 },
            {"extendedStyle",   required_argument, 0,  0 },
            {0,                 0,                 0,  0 }
        };

        c = getopt_long(argc, argv, "c:l:u:p:x::e",
                        long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 0:
        {
            if ( QString("host") == long_options[option_index].name )
                ::addArgument(arguments, "--host", QString::fromLocal8Bit(optarg));

            else if ( QString("port") == long_options[option_index].name )
                ::addArgument(arguments, "--port", QString::fromLocal8Bit(optarg));

            else if ( QString("extendedStyle") == long_options[option_index].name )
                ::addArgument(arguments, "--extendedStyle", QString::fromLocal8Bit(optarg));

            else
            {
                QString msg;
                msg = QString("option %1").arg(long_options[option_index].name);
                if (optarg)
                {
                    QString opt = QString::fromLocal8Bit(optarg);
                    msg += QString(" with arg %1").arg(opt);
                }
                qDebug() << qPrintable(commandLine) << qPrintable(msg);
            }
            break;
        }

        case 'c':
            ::addArgument(arguments, "-c", QString::fromLocal8Bit(optarg));
            break;

        case 'l':
            ::addArgument(arguments, "-l", QString::fromLocal8Bit(optarg));
            break;

        case 'u':
            ::addArgument(arguments, "-u", QString::fromLocal8Bit(optarg));
            break;

        case 'p':
            ::addArgument(arguments, "-p", QString::fromLocal8Bit(optarg));
            break;

        case 'e':
            ::addArgument(arguments, "-e", QString());
            break;

        case 'x':
        {
            QString optargString = QString::fromLocal8Bit(optarg);
            if (!optargString.isEmpty())
                settings.setValue(sysKeyOfStartApplication, optargString);
            else
                optargString = settings.value(sysKeyOfStartApplication, "").toString();

            ::addArgument(arguments, "-x", optargString);

            haveX = true;

            break;
        }
        case '?':
            break;

        default:
            qDebug("%s?? getopt returned character code 0%o ??", qPrintable(commandLine), c);
        }
    }

    if (!haveX)
        settings.setValue(sysKeyOfStartApplication, "");

    if (optind < argc)
    {
        QString msg("non-option ARGV-elements: ");
        while (optind < argc)
            msg += QString("%1 ").arg(argv[optind++]);
        qDebug() << qPrintable(commandLine) << qPrintable(msg);
    }

    return arguments;
}

// Convert command line arguments to predefined syntax arguments
static QStringList parseCommandLine(const QStringList &arguments)
{
    int argc = arguments.count();
    char **argv = new char*[argc];
    // Make argc/argv structure (prepare parameters for getopt function)
    for (int i = 0; i < argc; ++i)
    {
        QByteArray local8bit = arguments.at(i).toLocal8Bit();
        argv[i] = qstrdup(local8bit.constData());
    }

    QStringList args = parseCommandLine(argc, argv);

    // Release command line memory.
    for ( int i = 0; i < argc; i++ )
        delete [] argv[i];
    delete [] argv;

    return args;
}

static QString findArgument(const QStringList &args, const QString &option)
{
    // Find first option in the argument list.
    for ( int i = 0; i < args.size(); i++ )
    {
        if ( option == args[i] )
        {
            // Return string associated with an option.
            if ( i+1 < args.size() )
                return args[i+1];
            break;
        }
    }
    // Option is not found.
    return QString();
}


CmdLineArguments& CmdLineArguments::instance(const QStringList &args)
{
    static CmdLineArguments gCmdLine(args);
    return gCmdLine;
}

QString CmdLineArguments::usage()
{
    return QString("Usage: command line parameters\r\n"
                   "-c, --config        configuration file path\r\n"
                   "-l, --log           log file path\r\n"
                   "-u, --user          user name\r\n"
                   "-p, --password      password\r\n"
                   "-x, --xml           xml\r\n"
                   "--host              host address\r\n"
                   "--port              port number");
}

CmdLineArguments::CmdLineArguments()
{
}

CmdLineArguments::CmdLineArguments(const QStringList &args)
{
    // Parse UNIX style command line options to one style options.
    mArguments = parseCommandLine(args);
}

QString CmdLineArguments::username() const
{
    return ::findArgument(mArguments, "-u");
}

void CmdLineArguments::setUserName(const QString &name)
{
    ::addArgument(mArguments, "-u", name);
}

QString CmdLineArguments::password() const
{
    return ::findArgument(mArguments, "-p");
}

void CmdLineArguments::setPassword(const QString &pswd)
{
    ::addArgument(mArguments, "-p", pswd);
}

QString CmdLineArguments::log() const
{
    return ::findArgument(mArguments, "-l");
}

void CmdLineArguments::setLog(const QString &l)
{
    ::addArgument(mArguments, "-l", l);
}

QString CmdLineArguments::config() const
{
    return ::findArgument(mArguments, "-c");
}

void CmdLineArguments::setConfig(const QString &c)
{
    ::addArgument(mArguments, "-c", c);
}

QString CmdLineArguments::xml() const
{
    return ::findArgument(mArguments, "-x");
}

void CmdLineArguments::setXml(const QString &x)
{
    ::addArgument(mArguments, "-x", x);
}

QString CmdLineArguments::host() const
{
    return ::findArgument(mArguments, "--host");
}

void CmdLineArguments::setHost(const QString &h)
{
    ::addArgument(mArguments, "--host", h);
}

quint16 CmdLineArguments::port() const
{
    return ::findArgument(mArguments, "--port").toUInt();
}

void CmdLineArguments::setPort(quint16 p)
{
    ::addArgument(mArguments, "--port", QString::number(p));
}

QString CmdLineArguments::extendedStyle() const
{
    return ::findArgument(mArguments, "--extendedStyle");
}

void CmdLineArguments::setExtendedStyle(const QString &es)
{
    ::addArgument(mArguments, "--extendedStyle", es);
}
