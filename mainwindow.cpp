#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    refresh_list();
    scene = new QGraphicsScene(this);

    int left_margin = 10; // x coordinate
    int top_margin = 270; // y coordinate

    ui->graphicsView->setGeometry(left_margin, top_margin,
                                       BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene);

    scene->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);



}

MainWindow::~MainWindow()
{
    delete ui;
}
//Adds second and updates lcd number.
void MainWindow::second_gone()
{
    ++seconds;
    ui->lcdNumberSec->display(seconds);

}
//Adds moves and updates lcd number.
void MainWindow::add_move()
{
    ++moves;
    ui->lcdNumberMoves->display(moves);

}
/*Function initialyzes all the variables,
 *creates disks and starts timer .*/
void MainWindow::start_game()
{
    scene->clear();
    ui->labelWon->clear();

    //List are emptied.
    towerA = {};
    towerB = {};
    towerC = {};

    //Enables move buttons
    ui->pushButtonAtoB->setDisabled(false);
    ui->pushButtonAtoC->setDisabled(false);
    ui->pushButtonBtoA->setDisabled(false);
    ui->pushButtonBtoC->setDisabled(false);
    ui->pushButtonCtoA->setDisabled(false);
    ui->pushButtonCtoB->setDisabled(false);


    QBrush redBrush(Qt::red);   //color for disks
    QPen blackPen(Qt::black);   //disk line color

    int disk_height = 20;

    //Loop adds disks in the first pole.
    for ( int i = 1; i <= disk_number; ++i )
    {
        towerA.push_back(i);

        disk = scene->addRect(20+i*7,260-i*disk_height,200-i*14,disk_height, blackPen, redBrush);
        pointer_list.push_front(disk); //adds number of disk to check list.

    }
    //Counts smalles number of moves.
    QString min_moves = QString::number(pow(2, disk_number) - 1);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::second_gone);
    ui->labelInfo->setText("The minimal number of moves: " + min_moves);
    seconds = 0;
    moves = -1;
    add_move(); //moves are updated to 0
    timer->start(1000);


}
/*Funktion chances disk place in graphic scene
 * and changes the disk color*/
void MainWindow::move_disk(QList <int> &tower_from, QList <int> &tower_to, int distance, QBrush &color)
{
    if ( is_legal(tower_from, tower_to)) //if move legal
    {

        disk = get_disk(tower_from); //gets disks pinter
        int x = distance;   //how much disk is moved
        int y = (tower_from.size() - tower_to.size() - 1) * 20; //counts new y coordinate
        disk->moveBy(x,y); //moves to new coordinates
        disk->setBrush(color); //changes color

        add_move();
        tower_to.push_front(tower_from[0]); //adds disks number to list
        tower_from.removeFirst(); //removes disk number from lits
        if ( towerB.size() == disk_number || towerC.size() == disk_number) //checks if B or C tower is full
            won();

    }

}
//Function checks if move is legal
bool MainWindow::is_legal(QList <int> tower_from, QList <int> tower_to)
{

    if ( tower_from.size() == 0) //nothing to move
        return false;
    if ( tower_to.size() == 0) //new tower is empty
        return true;
    if (tower_from[0] > tower_to[0]) //first disk of new tower is smaller
    {
        return false;
    }else{                          //first disk of new tower is bigger
        return true;
    }
}
/*Function adds new winner to text file,
 * stops the timer, notifies for
 * win and disables move buttons.*/
void MainWindow::won()
{

    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
            return;

    //Writes winners info to textfile.
    QTextStream out(&file);
    QString text = player_name + ";" + QString::number(seconds) +
            ";" + QString::number(moves) + ";" + QString::number(disk_number)+ '\n';
    out << text;
    file.flush();
    file.close();

    refresh_list();
    timer->stop();
    ui->listWidget->clear();
    refresh_list();
    delete timer;

    ui->labelWon->setText("YOU WON!");
    /*Enables start button, spin button and line edit
     * so the new game can be sarteds.*/
    ui->pushButtonStart->setDisabled(false);
    ui->lineEdit->setDisabled(false);
    ui->spinBox->setDisabled(false);

    ui->pushButtonAtoB->setDisabled(true);
    ui->pushButtonAtoC->setDisabled(true);
    ui->pushButtonBtoA->setDisabled(true);
    ui->pushButtonBtoC->setDisabled(true);
    ui->pushButtonCtoA->setDisabled(true);
    ui->pushButtonCtoB->setDisabled(true);




}
/*Function reads file and adds it data to QMap.
 * QMaps data is then showed added listwidget*/
void MainWindow::refresh_list()
{
        QFile file(file_name);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return;
        //Reads file line by line splitting the text to list.
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList list = line.split(';');
            highscores[list[0]].append(list[1]); //time
            highscores[list[0]].append(list[2]); //moves
            highscores[list[0]].append(list[3]); //disks

        }
        //Adds QMaps data to list widget.
        QMap<QString, QList < QString > >::const_iterator i = highscores.constBegin();
        while (i != highscores.constEnd()) {
            QString player_data;
            player_data = i.key() + " | " + i.value()[0] + "s | " + i.value()[1] + " | " + i.value()[2];
            ui->listWidget->addItem(player_data);
            ++i;

        }

}
/*Function returns disks by using QLists first item
 * as a list index(first member of pointer_list is
smallest disk)*/
QGraphicsRectItem *MainWindow::get_disk(QList <int> tower)
{
    return pointer_list[(tower[0] - 1)];
}
/*Function clears text labels and checks inputs. if
 * inputs are good function disables buttons and
 * edits */
void MainWindow::on_pushButtonStart_clicked()
{
       player_name = ui->lineEdit->text();
       disk_number = ui->spinBox->value();
       if (player_name.length() < 1)
       {
           QMessageBox::information(this, "Warning!","Player name has to be atleast 1 charecter long!");
           return;
       }
       if (highscores.contains(player_name))
       {
           QMessageBox::information(this, "Warning!","Name " + player_name +" is taken!");
           return;
       }
       if ( disk_number < 1)
       {
           QMessageBox::information(this, "Warning!","Choose disk number between 1-9!");
           return;
       }
       ui->pushButtonStart->setDisabled(true);
       ui->lineEdit->setDisabled(true);
       ui->spinBox->setDisabled(true);
       start_game();

}
/*All functions below calll function move_disk.
 * All functions have a purpose to move disk to
 * tower and change disks color.*/


void MainWindow::on_pushButtonAtoB_clicked()
{
    QBrush yellowBrush(Qt::yellow);
    move_disk(towerA, towerB, 220, yellowBrush);
}


void MainWindow::on_pushButtonAtoC_clicked()
{
    QBrush greenBrush(Qt::green);
    move_disk(towerA, towerC, 440, greenBrush);
}

void MainWindow::on_pushButtonBtoA_clicked()
{
    QBrush redBrush(Qt::red);
    move_disk(towerB, towerA, -220, redBrush);
}
void MainWindow::on_pushButtonBtoC_clicked()
{
    QBrush greenBrush(Qt::green);
    move_disk(towerB, towerC, 220, greenBrush);
}

void MainWindow::on_pushButtonCtoA_clicked()
{
    QBrush redBrush(Qt::red);
    move_disk(towerC, towerA, -440, redBrush);
}

void MainWindow::on_pushButtonCtoB_clicked()
{
    QBrush yellowBrush(Qt::yellow);
    move_disk(towerC, towerB, -220, yellowBrush);
}
