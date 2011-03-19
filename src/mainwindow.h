#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <QtXml/QDomElement>
#include "ui_mainwindow.h"

class XMLCacheHandler;
class ConnectionData;
class QDomDocument;
class QString;
class QStringListModel;
class ConnectToServerDialog;
class MediaPlayer;
class QBuffer;

class MainWindow
    : public QMainWindow,
      public Ui::MainWindow
{
    Q_OBJECT

public:
    XMLCacheHandler *xch;
    ConnectionData *cd;
    QString currentArtist;

    QStringListModel *artistListModel;
    QStringListModel *albumListModel;
    QStringListModel *trackListModel;

    // Members
    QString serverpath;
    QString username;
    QString password;

    MediaPlayer *mediaPlayer;

    QBuffer *currentTrackBuffer;

    // Methods
    MainWindow();

    void setRequestConnections();

    QStringList *getValuesList(const QDomElement element,
                               const QString tagName,
                               const QString attributeName);

    QString *getValue(const QDomElement element,
                      const QString tagName,
                      const QString attributeName,
                      const QString attributeValue,
                      const QString returnAttributeName);


signals:
    void mediaPlay();
    void mediaPause();


private:
    void setMenuActions();
    void setPhonon();
    void setMediaActions();

public slots:
    void connectToServer();
    void about();
    void setServerData(QString&, QString&, QString&);
    void setTimeElapsedLabel(qint64); 

    void changeArtists(QDomElement);
    void requestAlbums(QModelIndex);
    void changeAlbums(QDomElement);
    void requestTracks(QModelIndex);
    void changeTracks(QDomElement);
};

#endif // MAINWINDOW_H
