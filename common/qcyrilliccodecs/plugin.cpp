
#include <QtCore/QTextCodecPlugin>

#include "allcodecs.h"

class QIta2Ia5TextCodecPlugin : public QTextCodecPlugin
{

public:
  QIta2Ia5TextCodecPlugin();
  virtual ~QIta2Ia5TextCodecPlugin();

  virtual QList<QByteArray>	aliases() const;
  virtual QTextCodec *	createForMib(int mib);
  virtual QTextCodec *	createForName(const QByteArray & name);
  virtual QList<int>	mibEnums() const;
  virtual QList<QByteArray>	names() const;
private:
  QITA2TextCodec ita2TextCodec;
  QKOI7TextCodec koi7Codec;
  MTK2Codec mtk2Codec;
  MTK5Codec mtk5Codec;
};

QList<QByteArray> QIta2Ia5TextCodecPlugin::aliases() const
{
  QList<QByteArray> result;

  result += ita2TextCodec.aliases();
  result += koi7Codec.aliases();
  result += mtk2Codec.aliases();
  result += mtk5Codec.aliases();

  return result;
}

QTextCodec *QIta2Ia5TextCodecPlugin::createForMib(int mib)
{
  if (koi7Codec.mibEnum() == mib) {
    return new QKOI7TextCodec;
  }
  return 0;
}

QTextCodec *QIta2Ia5TextCodecPlugin::createForName(const QByteArray &name)
{
  if (ita2TextCodec.name() == name) {
    return new QITA2TextCodec;
  } else if (koi7Codec.name() == name) {
    return new QKOI7TextCodec;
  } else if (mtk2Codec.name() == name) {
    return new MTK2Codec;
  } else if (mtk5Codec.name() == name) {
    return new MTK5Codec;
  }

  return 0;
}

QList<int> QIta2Ia5TextCodecPlugin::mibEnums() const
{
  QList<int> mibEnumsResult;
  //mibEnumsResult += ita2TextCodec.mibEnum();
  mibEnumsResult += koi7Codec.mibEnum();
  return mibEnumsResult;
}

QList<QByteArray> QIta2Ia5TextCodecPlugin::names() const
{
  QList<QByteArray> namesResult;
  namesResult += ita2TextCodec.name();
  namesResult += koi7Codec.name();
  namesResult += mtk2Codec.name();
  namesResult += mtk5Codec.name();
  return namesResult;
}

QIta2Ia5TextCodecPlugin::QIta2Ia5TextCodecPlugin()
  :QTextCodecPlugin()
{
}

QIta2Ia5TextCodecPlugin::~QIta2Ia5TextCodecPlugin()
{
}

Q_EXPORT_PLUGIN2(qcyrilliccodecs, QIta2Ia5TextCodecPlugin)
