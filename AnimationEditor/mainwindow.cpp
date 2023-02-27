#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

    for (QPixmap sprite : ui->spriteGrid)
    {

    }

    int maxRows = 3 + sprites.size() * 0.2;
    //Display all the sprites in a grid
    for(uint i = 0; i < sprites.size(); i++)
    {
        QLabel* spriteLabel = new QLabel(this);
        spriteLabel->setPixmap(sprites[i]);
        spriteLabel->setScaledContents( true );
        spriteLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

        ui->spriteGrid->addWidget(spriteLabel, i % maxRows, i / maxRows);
    }
}

