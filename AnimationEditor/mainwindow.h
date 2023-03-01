#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QIntValidator>
#include <QTimer>

#include "clickablelabel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void UpdateDisplay();
    void AnimationLoop();

private slots:
    void onSpriteClicked();
    void on_actionOpen_triggered();
    void on_addFrame_clicked();
    void on_frameSlider_sliderMoved(int position);
    void on_backwards_clicked();
    void on_forwards_clicked();
    void on_delay_editingFinished();
    void on_removeFrame_clicked();
    void on_play_clicked();

private:
    QTimer *timer;
    Ui::MainWindow *ui;
    std::vector<QPixmap> sprites;
    std::vector<ClickableLabel*> spriteLabels;

    bool playingAnimation;
    int animationMsElapsed;

    int selectedSprite = 0;
    int currentFrame = -1;
    int frames = 0;
    //ID of the sprite for each frame
    std::vector<int> frameSprites;
    //Delays after each frame in ms
    std::vector<int> frameDelays;
};
#endif // MAINWINDOW_H
