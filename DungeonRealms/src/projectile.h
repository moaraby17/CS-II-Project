#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QPoint>

// Small value class used by MainWindow when it draws a projectile animation.
class Projectile {
public:
    Projectile(QPoint startTile = QPoint(), QPoint endTile = QPoint(), bool fromPlayer = true);

    QPoint startTile() const;
    QPoint endTile() const;
    bool isFromPlayer() const;

private:
    QPoint m_startTile;
    QPoint m_endTile;
    bool m_fromPlayer;
};

#endif // PROJECTILE_H
