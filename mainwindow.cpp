#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include "gamecontroller.h"
#include "scenetitle.h"
#define TILE_SIZE 32

/*
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
*/
// initialization of window
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    //scene(new QGraphicsScene(this)),
    system(new GameController(this))
{
    setCentralWidget(system->view);
    // fix the window to 1280*720.
    setFixedSize(1280, 720);
    // set the title.
    setWindowTitle("The Strategist");
    QTimer::singleShot(0, this, SLOT(adjustViewSize()));
}

MainWindow::~MainWindow()
{

}

/*
void MainWindow::refresh()
{
    // refresh the scene
    scene->refresh();
}
*/

/*
void MainWindow::initScene()
{
    scene->setSceneRect(0, 0, 1280, 720);
}
void MainWindow::initSceneBackground()
{
    QPixmap bg(":/Scene_Title.jpg");
    QPainter p(&bg);
    p.setBrush(QBrush(Qt::gray));
    p.drawRect(0, 0, 1280, 720);

    scene->setBackgroundBrush(QBrush(bg));
    QImage image(":/Graphics/SceneTitle.png");

    QGraphicsPixmapItem item(QPixmap::fromImage(image));
    scene->addItem(&item);
}
*/


