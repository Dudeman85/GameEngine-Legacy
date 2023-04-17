#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->delay->setValidator( new QIntValidator(1, 100000000, this) );
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::AnimationLoop);
}
MainWindow::~MainWindow()
{
    delete ui;
}

//Updates some GUI elements
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
        ui->frameSlider->setMaximum(0);
        ui->frameSlider->setSliderPosition(0);
    }
}

//Update the bottom timeline
void MainWindow::UpdateTimeline()
{
    //Delete the old labels
    for(QLabel* label : timelineLabels)
        delete label;
    for(QLineEdit* delay : timelineDelays)
        delete delay;
    timelineLabels.clear();
    timelineDelays.clear();

    //Add the frames to the bottom timeline
    for (int i = 0; i < frames; i++)
    {
        //Make the label for displaying the sprite
        QLabel* spriteLabel = new QLabel(this);
        spriteLabel->setPixmap(sprites[frameSprites[i]]);
        spriteLabel->setScaledContents( true );
        spriteLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

        //Store the labels so they can be deleted
        timelineLabels.push_back(spriteLabel);

        //Make the line edit holding the frame delay
        QLineEdit* frameDelay = new QLineEdit();
        frameDelay->setText(QString::number(frameDelays[i]));
        frameDelay->setValidator( new QIntValidator(1, 100000000, this) );
        connect(frameDelay, SIGNAL(editingFinished()), this, SLOT(onFrameDelayChanged()));

        //Store the line edits so they can be referenced
        timelineDelays.push_back(frameDelay);

        //Add the sprite and delay to the timeline grid
        ui->timelineGrid->addWidget(spriteLabel, 0, i, Qt::AlignCenter);
        ui->timelineGrid->addWidget(frameDelay, 1, i);
    }
}

//Animation display loop, called every 1ms by the timer
void MainWindow::AnimationLoop()
{
    animationMsElapsed++;

    //Switch the current frame of the animation once enough time has passed
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

    //Ratio of rows to columns
    const float ratio = 3.f / 2.f;
    int maxRows = ceil(sqrt(sprites.size() * ratio));
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

//Remove all images
void MainWindow::on_actionRemove_All_triggered()
{
    animationTimer->stop();

    //Stop the animation
    QFont font = QFont(ui->play->font());
    font.setPointSize(18);
    ui->play->setFont(font);
    ui->play->setText("►");

    //Delete all the old labels
    for (ClickableLabel* label : spriteLabels)
    {
        delete label;
    }

    //Delete everything and reset variables to default
    sprites.clear();
    spriteLabels.clear();
    for (int i = 0; i < frames; i++)
    {
        delete timelineLabels[i];
        delete timelineDelays[i];
    }
    timelineLabels.clear();
    timelineDelays.clear();
    selectedSprite = 0;
    frames = 0;
    currentFrame = -1;

    UpdateDisplay();
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

//When a frame delay is changed on the timeline update them
void MainWindow::onFrameDelayChanged()
{
    for (int i = 0; i < frames; i++)
    {
        frameDelays[i] = timelineDelays[i]->text().toInt();
    }
}

//When add frame button is clicked
void MainWindow::on_addFrame_clicked()
{
    //If no sprites have been loaded do nothing
    if(sprites.empty())
        return;

    currentFrame++;
    frameSprites.insert(frameSprites.begin() + currentFrame, selectedSprite);
    frameDelays.insert(frameDelays.begin() + currentFrame, ui->delay->text().toInt());
    frames++;

    //Stop Animation
    animationTimer->stop();
    playingAnimation = false;

    //Change play button text
    QFont font = QFont(ui->play->font());
    font.setPointSize(18);
    ui->play->setFont(font);
    ui->play->setText("►");

    UpdateTimeline();
    UpdateDisplay();
}
//When remove frame button is clicked
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

    //Stop Animation
    animationTimer->stop();
    playingAnimation = false;

    //Change play button text
    QFont font = QFont(ui->play->font());
    font.setPointSize(18);
    ui->play->setFont(font);
    ui->play->setText("►");

    UpdateTimeline();
    UpdateDisplay();
}

//Timeline control buttons and slider
void MainWindow::on_backwards_clicked()
{
    //Recede frame, loop to end if necessary
    if(currentFrame > 0)
        currentFrame--;
    else
        currentFrame = frames - 1;

    UpdateDisplay();
}
void MainWindow::on_forwards_clicked()
{
    //Advance frame, loop to beginning if necessary
    if(currentFrame < frames - 1)
        currentFrame++;
    else
        currentFrame = 0;

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

    //Start or stop the animation clock
    if(!playingAnimation)
    {
        animationTimer->start(1);

        //Change play button text
        QFont font = QFont(ui->play->font());
        font.setPointSize(8);
        ui->play->setFont(font);
        ui->play->setText(" ▌▌");
    }
    else
    {
        animationTimer->stop();

        //Change play button text
        QFont font = QFont(ui->play->font());
        font.setPointSize(18);
        ui->play->setFont(font);
        ui->play->setText("►");
    }

    playingAnimation = !playingAnimation;
}
void MainWindow::on_delay_editingFinished()
{
    //If no frames have been added do nothing
    if(frameDelays.empty())
        return;

    frameDelays[currentFrame] = ui->delay->text().toInt();
}
