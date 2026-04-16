#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QVector>

#include "game.h"

class QGraphicsRectItem;
class QGraphicsScene;
class QGraphicsSimpleTextItem;
class QGraphicsView;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void renderGame();
    void showLevelCompleteMessage();

private:
    void setupUi();
    QColor tileColor(TileType type) const;

    static constexpr int TILE_SIZE = 48;

    Game m_game;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
    QLabel* m_hpLabel;
    QLabel* m_statusLabel;

    QVector<QGraphicsRectItem*> m_tileRects;
    QVector<QGraphicsSimpleTextItem*> m_tileTexts;
};

#endif // MAINWINDOW_H
