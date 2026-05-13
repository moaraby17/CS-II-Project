#include "mainwindow.h"

#include "projectile.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QVariant>
#include <QVariantAnimation>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_view(nullptr),
      m_scene(nullptr),
      m_levelLabel(nullptr),
      m_hpLabel(nullptr),
      m_objectiveLabel(nullptr),
      m_statusLabel(nullptr) {
    setupUi();

    connect(&m_game, &Game::gameStateChanged, this, &MainWindow::renderGame);
    connect(&m_game, &Game::levelCompleted, this, &MainWindow::showLevelCompleteMessage);
    connect(&m_game, &Game::gameWon, this, &MainWindow::showVictoryMessage);
    connect(&m_game, &Game::playerDied, this, &MainWindow::showDeathMessage);
    connect(&m_game, &Game::projectileFired, this, &MainWindow::animateProjectile, Qt::QueuedConnection);

    renderGame();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    // Hidden demo helper: jump forward one level without changing normal controls.
    if (event->key() == Qt::Key_N
        && event->modifiers().testFlag(Qt::ControlModifier)
        && event->modifiers().testFlag(Qt::ShiftModifier)) {
        m_game.skipToNextLevel();
        return;
    }

    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        m_game.handlePlayerMove(0, -1);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        m_game.handlePlayerMove(0, 1);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        m_game.handlePlayerMove(-1, 0);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        m_game.handlePlayerMove(1, 0);
        break;
    case Qt::Key_Space:
    case Qt::Key_F:
        m_game.handlePlayerAttack();
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void MainWindow::renderGame() {
    const Level& level = m_game.level();
    const Player& player = m_game.player();

    // Stop old projectile animations before clearing the scene so no animation
    // tries to move an item that has already been deleted.
    for (QVariantAnimation* animation : m_projectileAnimations) {
        animation->stop();
        animation->deleteLater();
    }
    m_projectileAnimations.clear();
    m_projectiles.clear();

    m_scene->clear();
    m_tileRects.clear();
    m_tileTexts.clear();
    m_scene->setSceneRect(0, 0, level.width() * TILE_SIZE, level.height() * TILE_SIZE);

    for (int y = 0; y < level.height(); ++y) {
        for (int x = 0; x < level.width(); ++x) {
            const QRectF rect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            QGraphicsRectItem* tileRect = m_scene->addRect(rect, QPen(Qt::black), QBrush(tileColor(level.tileTypeAt(x, y))));
            m_tileRects.append(tileRect);

            const QString text = tileLetter(level.tileTypeAt(x, y));
            if (!text.isEmpty()) {
                QGraphicsSimpleTextItem* item = m_scene->addSimpleText(text);
                item->setBrush(QBrush(Qt::black));
                item->setPos(x * TILE_SIZE + 17, y * TILE_SIZE + 12);
                m_tileTexts.append(item);
            }
        }
    }

    for (const Enemy& enemy : level.enemies()) {
        if (!enemy.isAlive()) {
            continue;
        }

        const QRectF enemyRect(enemy.x() * TILE_SIZE + 8, enemy.y() * TILE_SIZE + 8, TILE_SIZE - 16, TILE_SIZE - 16);
        m_scene->addEllipse(enemyRect, QPen(Qt::darkRed), QBrush(enemyColor(enemy.type())));

        QGraphicsSimpleTextItem* enemyText = m_scene->addSimpleText(enemy.displayLetter());
        enemyText->setBrush(QBrush(Qt::white));
        enemyText->setPos(enemy.x() * TILE_SIZE + 18, enemy.y() * TILE_SIZE + 12);
    }

    const QRectF playerRect(player.x() * TILE_SIZE + 8, player.y() * TILE_SIZE + 8, TILE_SIZE - 16, TILE_SIZE - 16);
    m_scene->addEllipse(playerRect, QPen(Qt::darkBlue), QBrush(Qt::blue));

    QGraphicsSimpleTextItem* playerText = m_scene->addSimpleText("P");
    playerText->setBrush(QBrush(Qt::white));
    playerText->setPos(player.x() * TILE_SIZE + 18, player.y() * TILE_SIZE + 12);

    m_levelLabel->setText(QString("Level %1 / %2: %3")
                              .arg(m_game.currentLevel())
                              .arg(m_game.totalLevels())
                              .arg(level.name()));
    m_hpLabel->setText(QString("HP: %1 / %2   Key: %3")
                           .arg(player.health())
                           .arg(player.maxHealth())
                           .arg(player.hasKey() ? "Yes" : "No"));
    m_objectiveLabel->setText(QString("Enemies remaining: %1   Objective: defeat enemies, then reach X")
                                  .arg(m_game.enemiesRemaining()));
    m_statusLabel->setText(m_game.statusText());
}

void MainWindow::showLevelCompleteMessage(int levelNumber) {
    if (levelNumber < m_game.totalLevels()) {
        QMessageBox::information(this, "Level Complete", QString("Level %1 complete! Entering Level %2.")
                                                          .arg(levelNumber)
                                                          .arg(levelNumber + 1));
    }
}

void MainWindow::showVictoryMessage() {
    QMessageBox::information(this, "Victory", "Victory! You cleared all five levels of Dungeon Realms!");
}

void MainWindow::showDeathMessage(int levelNumber) {
    QMessageBox::warning(this, "You Died", QString("You died! Level %1 will restart from the beginning.").arg(levelNumber));
}

void MainWindow::animateProjectile(int fromX, int fromY, int toX, int toY, bool fromPlayer) {
    const Projectile shot(QPoint(fromX, fromY), QPoint(toX, toY), fromPlayer);
    const QPointF start(shot.startTile().x() * TILE_SIZE + TILE_SIZE / 2.0,
                        shot.startTile().y() * TILE_SIZE + TILE_SIZE / 2.0);
    const QPointF end(shot.endTile().x() * TILE_SIZE + TILE_SIZE / 2.0,
                      shot.endTile().y() * TILE_SIZE + TILE_SIZE / 2.0);
    QGraphicsEllipseItem* projectile = m_scene->addEllipse(-5, -5, 10, 10,
                                                           QPen(Qt::black),
                                                           QBrush(shot.isFromPlayer() ? Qt::yellow : Qt::magenta));
    projectile->setPos(start);
    m_projectiles.append(projectile);

    auto* animation = new QVariantAnimation(this);
    m_projectileAnimations.append(animation);
    animation->setDuration(220);
    animation->setStartValue(start);
    animation->setEndValue(end);
    connect(animation, &QVariantAnimation::valueChanged, this, [projectile](const QVariant& value) {
        projectile->setPos(value.toPointF());
    });
    connect(animation, &QVariantAnimation::finished, this, [this, projectile, animation]() {
        m_projectiles.removeOne(projectile);
        m_projectileAnimations.removeOne(animation);
        if (projectile->scene()) {
            m_scene->removeItem(projectile);
        }
        delete projectile;
        animation->deleteLater();
    });
    animation->start();
}

void MainWindow::setupUi() {
    setWindowTitle("Dungeon Realms - Final Demo");
    resize(700, 660);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    m_levelLabel = new QLabel(this);
    m_hpLabel = new QLabel(this);
    m_objectiveLabel = new QLabel(this);
    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);

    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);

    const int width = 10 * TILE_SIZE;
    const int height = 10 * TILE_SIZE;
    m_scene->setSceneRect(0, 0, width, height);
    m_view->setFixedSize(width + 2, height + 2);

    layout->addWidget(m_levelLabel);
    layout->addWidget(m_hpLabel);
    layout->addWidget(m_objectiveLabel);
    layout->addWidget(m_statusLabel);
    layout->addWidget(m_view);

    setCentralWidget(central);
}

QColor MainWindow::tileColor(TileType type) const {
    switch (type) {
    case TileType::Floor:
        return QColor(220, 220, 220);
    case TileType::Wall:
        return QColor(80, 80, 80);
    case TileType::Trap:
        return QColor(245, 180, 90);
    case TileType::Goal:
        return QColor(130, 230, 130);
    case TileType::LockedDoor:
        return QColor(145, 95, 45);
    case TileType::Key:
        return QColor(250, 235, 120);
    }

    return Qt::white;
}

QString MainWindow::tileLetter(TileType type) const {
    switch (type) {
    case TileType::Trap:
        return "^";
    case TileType::Goal:
        return "X";
    case TileType::LockedDoor:
        return "D";
    case TileType::Key:
        return "K";
    default:
        return QString();
    }
}

QColor MainWindow::enemyColor(EnemyType type) const {
    switch (type) {
    case EnemyType::Basic:
        return QColor(210, 40, 40);
    case EnemyType::Archer:
        return QColor(150, 50, 210);
    case EnemyType::Brute:
        return QColor(110, 20, 20);
    }
    return Qt::red;
}
