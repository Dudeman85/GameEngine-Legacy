#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->delay->setValidator( new QIntValidator(1, 100000000, this) );
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::AnimationLoop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Updates all GUI elements
void MainWindow::UpdateDisplay()
{
    if(frames > 0)
    {
        ui->mainScene->setPixmap(sprites[frameSprites[currentFrame]]);
        ui->frameSlider->setMaximum(std::max(0, frames - 1));
        ui->frameSlider->setSliderPosition(currentFrame);
        ui->delay->setText(QString::number(frameDelays[currentFrame]));
    }
    else
    {
        ui->mainScene->clear();
    }
}

void MainWindow::AnimationLoop()
{
    animationMsElapsed++;

    if(animationMsElapsed >= frameDelays[currentFrame])
    {
        animationMsElapsed = 0;
        currentFrame++;
        if(currentFrame >= frames)
            currentFrame = 0;

        UpdateDisplay();
    }
}

//Load sprite images
void MainWindow::on_actionOpen_triggered()
{
    //Create the select files dialog
    QFileDialog selectSpriteDialog(this);
    selectSpriteDialog.setFileMode(QFileDialog::ExistingFiles);
    selectSpriteDialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    selectSpriteDialog.setDirectory("C:/Users/AleksiAnd/Documents/QT/tiimi4/AnimationEditor");

    //Open the dialog and get all the file paths
    QStringList filePaths;
    if (selectSpriteDialog.exec())
        filePaths = selectSpriteDialog.selectedFiles();

    //For each file opened
    for (const QString &filePath : filePaths)
    {
        QPixmap image(filePath);
        sprites.push_back(image);
    }

    //Delete all the old labels
    for (ClickableLabel* label : spriteLabels)
    {
        delete label;
    }
    spriteLabels.clear();

    //TODO Alogrithm
    int maxRows = 3 + sprites.size() * 0.2;
    //Display all the sprites in a grid
    for(uint i = 0; i < sprites.size(); i++)
    {
        //Make the clickable label for displaying the sprites
        ClickableLabel* spriteLabel = new ClickableLabel(this);
        spriteLabel->ID = i;
        spriteLabel->setPixmap(sprites[i]);
        spriteLabel->setScaledContents( true );
        spriteLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        connect(spriteLabel, SIGNAL(clicked()), this, SLOT(onSpriteClicked()));

        //Store the labels so they can be deleted
        spriteLabels.push_back(spriteLabel);

        //Add the label to the display grid
        ui->spriteGrid->addWidget(spriteLabel, i % maxRows, i / maxRows);
    }

    spriteLabels[selectedSprite]->setStyleSheet("border: 2px solid black;");
}

//Select the sprite when clicked
void MainWindow::onSpriteClicked()
{
    ClickableLabel *label = (ClickableLabel*)sender();

    //Remove the previously selected sprite border
    spriteLabels[selectedSprite]->setStyleSheet("border: 0px solid black;");

    //Add a border and select the clicked sprite
    label->setStyleSheet("border: 2px solid black;");
    selectedSprite = label->ID;
}

//When add frame butten is clicked
void MainWindow::on_addFrame_clicked()
{
    //If no sprites have been loaded do nothing
    if(sprites.empty())
        return;

    currentFrame++;
    frameSprites.insert(frameSprites.begin() + currentFrame, selectedSprite);
    frameDelays.insert(frameDelays.begin() + currentFrame, ui->delay->text().toInt());
    frames++;

    qDebug() << frameSprites;
    qDebug() << frameDelays;
    UpdateDisplay();
}
void MainWindow::on_removeFrame_clicked()
{
    //If no frames have been added do nothing
    if(frames == 0)
        return;

    frameSprites.erase(frameSprites.begin() + currentFrame);
    frameDelays.erase(frameDelays.begin() + currentFrame);
    frames--;
    if(currentFrame > 0)
        currentFrame--;
    if(frames == 0)
        currentFrame = -1;

    UpdateDisplay();
}
void MainWindow::on_delay_editingFinished()
{
    //If no frames have been added do nothing
    if(frameDelays.empty())
        return;

    frameDelays[currentFrame] = ui->delay->text().toInt();
}

//Timeline control buttons and slider
void MainWindow::on_backwards_clicked()
{
    if(currentFrame > 0)
        currentFrame--;
    UpdateDisplay();
}
void MainWindow::on_forwards_clicked()
{
    if(currentFrame < frames - 1)
        currentFrame++;
    UpdateDisplay();
}
void MainWindow::on_frameSlider_sliderMoved(int position)
{
    currentFrame = position;
    UpdateDisplay();
}
void MainWindow::on_play_clicked()
{
    //If less than 2 frames have been added do nothing
    if(frames < 2)
        return;

    if(!playingAnimation)
        timer->start(1);
    else
        timer->stop();

    playingAnimation = !playingAnimation;
}

