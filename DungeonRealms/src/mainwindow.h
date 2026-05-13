#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QVector>

#include "game.h"

class QGraphicsItem;
class QGraphicsRectItem;
class QGraphicsScene;
class QGraphicsSimpleTextItem;
class QGraphicsView;
class QVariantAnimation;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void renderGame();
    void showLevelCompleteMessage(int levelNumber);
    void showVictoryMessage();
    void showDeathMessage(int levelNumber);
    void animateProjectile(int fromX, int fromY, int toX, int toY, bool fromPlayer);

private:
    void setupUi();
    QColor tileColor(TileType type) const;
    QString tileLetter(TileType type) const;
    QColor enemyColor(EnemyType type) const;

    static constexpr int TILE_SIZE = 48;

    Game m_game;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
    QLabel* m_levelLabel;
    QLabel* m_hpLabel;
    QLabel* m_objectiveLabel;
    QLabel* m_statusLabel;

    QVector<QGraphicsRectItem*> m_tileRects;
    QVector<QGraphicsSimpleTextItem*> m_tileTexts;
    QVector<QGraphicsItem*> m_projectiles;
    QVector<QVariantAnimation*> m_projectileAnimations;
};

#endif // MAINWINDOW_H
