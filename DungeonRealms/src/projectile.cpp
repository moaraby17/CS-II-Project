#include "projectile.h"

Projectile::Projectile(QPoint startTile, QPoint endTile, bool fromPlayer)
    : m_startTile(startTile), m_endTile(endTile), m_fromPlayer(fromPlayer) {}

QPoint Projectile::startTile() const {
    return m_startTile;
}

QPoint Projectile::endTile() const {
    return m_endTile;
}

bool Projectile::isFromPlayer() const {
    return m_fromPlayer;
}
