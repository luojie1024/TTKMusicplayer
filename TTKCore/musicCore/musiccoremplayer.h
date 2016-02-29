#ifndef MUSICCOREMPLAYER_H
#define MUSICCOREMPLAYER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include "musicglobaldefine.h"

class QProcess;

/*! @brief The class of the mplayer core.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicCoreMPlayer : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        StoppedState,   ///*stop state*/
        PlayingState,   ///*play state*/
        PausedState     ///*pause state*/
    };

    enum Category
    {
        NullCategory,   ///*no category*/
        MusicCategory,  ///*music category*/
        RadioCategory,  ///*radio category*/
        VideoCategory   ///*video category*/
    };

    explicit MusicCoreMPlayer(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicCoreMPlayer();

    void setMedia(Category type, const QString &data, int winId);
    /*!
     * Set media by type and data path.
     */
    void setPosition(qint64 pos);
    /*!
     * Set current play pos.
     */

    void setMute(bool mute);
    /*!
     * Set current player mute or not.
     */
    void setVolume(int value);
    /*!
     * Set current player volume.
     */
    inline State state() { return m_playState;}
    /*!
     * Get current player state.
     */

Q_SIGNALS:
    void positionChanged(qint64 position);
    /*!
     * Current position changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current duration changed.
     */
    void volumnChanged(int volumn);
    /*!
     * Current volumn changed.
     */
    void mediaChanged(const QString &data);
    /*!
     * Current media data path changed.
     */
    void stateChanged(State state);
    /*!
     * Current state changed.
     */
    void radioChanged();
    /*!
     * Current media is radio changed.
     */
    void musicChanged();
    /*!
     * Current media is music changed.
     */

public Q_SLOTS:
    void play();
    /*!
     * Player to play.
     */
    void stop();
    /*!
     * Player to stop.
     */

private Q_SLOTS:
    void dataRecieve();
    /*!
     * Player data has recieved.
     */
    void positionRecieve();
    /*!
     * Player position data has recieved.
     */
    void durationRecieve();
    /*!
     * Player duration data has recieved.
     */
    void radioStandardRecieve();
    /*!
     * Player radio data has recieved.
     */
    void musicStandardRecieve();
    /*!
     * Player music data has recieved.
     */
    void timeout();
    /*!
     * Player one second time out.
     */

protected:
    void setVideoMedia(const QString &data, int winId);
    /*!
     * Set player to video media data.
     */
    void setMusicMedia(const QString &data);
    /*!
     * Set player to music media data.
     */
    void setRadioMedia(const QString &data);
    /*!
     * Set player to radio media data.
     */

    QProcess *m_process;
    State m_playState;
    Category m_category;
    QTimer m_timer;
    int m_currentPos;

};

#endif // MUSICCOREMPLAYER_H