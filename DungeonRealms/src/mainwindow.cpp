#include "mainwindow.h"

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_view(nullptr), m_scene(nullptr), m_hpLabel(nullptr), m_statusLabel(nullptr) {
    setupUi();

    connect(&m_game, &Game::gameStateChanged, this, &MainWindow::renderGame);
    connect(&m_game, &Game::levelCompleted, this, &MainWindow::showLevelCompleteMessage);

    renderGame();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
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
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void MainWindow::renderGame() {
    const Level& level = m_game.level();
    const Player& player = m_game.player();

    m_scene->clear();
    m_tileRects.clear();
    m_tileTexts.clear();

    for (int y = 0; y < level.height(); ++y) {
        for (int x = 0; x < level.width(); ++x) {
            const QRectF rect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            QGraphicsRectItem* tileRect = m_scene->addRect(rect, QPen(Qt::black), QBrush(tileColor(level.tileTypeAt(x, y))));
            m_tileRects.append(tileRect);

            QString text;
            if (level.tileTypeAt(x, y) == TileType::Trap) {
                text = "T";
            } else if (level.tileTypeAt(x, y) == TileType::Goal) {
                text = "G";
            }

            if (!text.isEmpty()) {
                QGraphicsSimpleTextItem* item = m_scene->addSimpleText(text);
                item->setBrush(QBrush(Qt::black));
                item->setPos(x * TILE_SIZE + 18, y * TILE_SIZE + 12);
                m_tileTexts.append(item);
            }
        }
    }

    for (const Enemy& enemy : level.enemies()) {
        if (!enemy.isAlive()) {
            continue;
        }

        const QRectF enemyRect(enemy.x() * TILE_SIZE + 8, enemy.y() * TILE_SIZE + 8, TILE_SIZE - 16, TILE_SIZE - 16);
        m_scene->addEllipse(enemyRect, QPen(Qt::darkRed), QBrush(Qt::red));

        QGraphicsSimpleTextItem* enemyText = m_scene->addSimpleText("E");
        enemyText->setBrush(QBrush(Qt::white));
        enemyText->setPos(enemy.x() * TILE_SIZE + 18, enemy.y() * TILE_SIZE + 12);
    }

    const QRectF playerRect(player.x() * TILE_SIZE + 8, player.y() * TILE_SIZE + 8, TILE_SIZE - 16, TILE_SIZE - 16);
    m_scene->addEllipse(playerRect, QPen(Qt::darkBlue), QBrush(Qt::blue));

    QGraphicsSimpleTextItem* playerText = m_scene->addSimpleText("P");
    playerText->setBrush(QBrush(Qt::white));
    playerText->setPos(player.x() * TILE_SIZE + 18, player.y() * TILE_SIZE + 12);

    m_hpLabel->setText(QString("HP: %1 / %2").arg(player.health()).arg(player.maxHealth()));
    m_statusLabel->setText(m_game.statusText());
}

void MainWindow::showLevelCompleteMessage() {
    QMessageBox::information(this, "Victory", "Level 1 Complete!");
}

void MainWindow::setupUi() {
    setWindowTitle("Dungeon Realms - Milestone 1");
    resize(650, 620);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    m_hpLabel = new QLabel(this);
    m_statusLabel = new QLabel(this);

    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);

    const int width = m_game.level().width() * TILE_SIZE;
    const int height = m_game.level().height() * TILE_SIZE;

    m_scene->setSceneRect(0, 0, width, height);
    m_view->setFixedSize(width + 2, height + 2);

    layout->addWidget(m_hpLabel);
    layout->addWidget(m_statusLabel);
    layout->addWidget(m_view);

    setCentralWidget(central);
}

QColor MainWindow::tileColor(TileType type) const {
    switch (type) {
    case TileType::Floor:
        return QColor(220, 220, 220);
    case TileType::Wall:
        return QColor(90, 90, 90);
    case TileType::Trap:
        return QColor(245, 200, 120);
    case TileType::Goal:
        return QColor(170, 240, 170);
    }

    return Qt::white;
}
