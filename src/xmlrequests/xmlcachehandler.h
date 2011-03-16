#ifndef XMLCACHEHANDLER_H
#define XMLCACHEHANDLER_H
#include <QtXml/QDomDocument>
#include <QObject>

class XMLCacheHandler: public QObject
{
    Q_OBJECT

public:
    XMLCacheHandler(QObject* parent);
    ~XMLCacheHandler();

signals:
    void newCacheCreated();

public slots:
    void saveNewCache();

private:
    QDomDocument* cacheFile;
    void checkCacheAge();
    void getNewIndexes();

    XMLCacheHandler();

};

#endif // XMLCACHEHANDLER_H
