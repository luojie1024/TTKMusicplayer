#include "musicsongsearchonlinewidget.h"
#include "musictextdownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicdata2downloadthread.h"
#include "musicbgthemedownload.h"
#include "musicmydownloadrecordobject.h"
#include "musiclocalsongsearchrecordobject.h"

#include <QDateTime>

MusicSongSearchOnlineWidget::MusicSongSearchOnlineWidget(QWidget *parent) :
    MusicTableQueryWidget(parent)
{
    setColumnCount(6);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0,30);
    headerview->resizeSection(1,215);
    headerview->resizeSection(2,161);
    headerview->resizeSection(3,50);
    headerview->resizeSection(4,26);
    headerview->resizeSection(5,26);
    setTransparent(255);
}

MusicSongSearchOnlineWidget::~MusicSongSearchOnlineWidget()
{
    clearAllItems();
}

void MusicSongSearchOnlineWidget::startSearchQuery(const QString &text)
{
    ////////////////////////////////////////////////
    QStringList names, times;
    MusicLocalSongSearchRecordObject search(this);
    if(!search.readSearchXMLConfig()) return;
    search.readSearchConfig(names, times);
    names.insert(0, text);
    times.insert(0, QString::number(QDateTime::currentMSecsSinceEpoch()));
    search.writeSearchConfig(names, times);
    ////////////////////////////////////////////////
    m_downLoadManager->startSearchSong(Music, text);
}

void MusicSongSearchOnlineWidget::clearAllItems()
{
    MusicTableWidgetAbstract::clearAllItems();
    setColumnCount(6);
}

void MusicSongSearchOnlineWidget::creatSearchedItems(const QString &songname,
                          const QString &artistname, const QString &time)
{
    int count;
    setRowCount(count = m_downLoadManager->getSongIdIndex());

    QTableWidgetItem *item = new QTableWidgetItem(QString::number( count ));
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 0, item);

                      item = new QTableWidgetItem(songname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 1, item);

                      item = new QTableWidgetItem(artistname);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 2, item);

                      item = new QTableWidgetItem(time);
    item->setTextColor(QColor(50,50,50));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 3, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/image/addtoplaylist")),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 4, item);

                      item = new QTableWidgetItem(QIcon(QString::fromUtf8(":/share/musicdownload")),"");
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count - 1, 5, item);
}

void MusicSongSearchOnlineWidget::listCellClicked(int row,int col)
{
    switch(col)
    {
      case 4:
        addSearchMusicToPlayList(row);break;
      case 5:
        musicDownloadLocal(row);break;
      default:break;
    }
}

void MusicSongSearchOnlineWidget::addSearchMusicToPlayList(int row)
{
    emit showDownLoadInfoFor(Buffing);
    musicDownloadLocal(row);
    emit MuiscSongToPlayListChanged( item(row, 2)->text() + " - " + item(row, 1)->text() );
}

void MusicSongSearchOnlineWidget::musicDownloadLocal(int row)
{
    emit showDownLoadInfoFor(DownLoading);
    MStringLists musicSongInfo(m_downLoadManager->getMusicSongInfo());
    QString musicSong =  item(row, 2)->text() + " - " + item(row, 1)->text() ;
    QString downloadName = MUSIC_DOWNLOAD + musicSong + MUSIC_FILE;

    ////////////////////////////////////////////////
    QStringList name, path;
    MusicMyDownloadRecordObject down(this);
    if(!down.readDownloadXMLConfig())
        return;
    down.readDownloadConfig(name,path);
    name<<musicSong;
    path<<QFileInfo(downloadName).absoluteFilePath();
    down.writeDownloadConfig(name,path);
    ////////////////////////////////////////////////

    MusicDataDownloadThread *downSong = new MusicDataDownloadThread(
                                            musicSongInfo[row][0], downloadName,this);
    connect(downSong,SIGNAL(musicDownLoadFinished(QString)),this,
                     SIGNAL(showDownLoadInfoFinished(QString)));
    downSong->startToDownload();

    (new MusicTextDownLoadThread(musicSongInfo[row][1],LRC_DOWNLOAD +
                                 musicSong + LRC_FILE,this))->startToDownload();
    (new MusicData2DownloadThread(musicSongInfo[row][2],
          ART_DOWNLOAD + musicSongInfo[row][3] + SKN_FILE,this))->startToDownload();

    ///download big picture
    new MusicBgThemeDownload(musicSongInfo[row][3], musicSongInfo[row][3], this);
}

void MusicSongSearchOnlineWidget::itemDoubleClicked(int row, int)
{
    addSearchMusicToPlayList(row);
}
