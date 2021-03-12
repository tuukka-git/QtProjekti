#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTimer>
#include <cmath>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();





private slots:
    //Button to move disks.
    void on_pushButtonStart_clicked();

    void on_pushButtonAtoB_clicked();

    void on_pushButtonAtoC_clicked();

    void on_pushButtonBtoA_clicked();

    void on_pushButtonBtoC_clicked();

    void on_pushButtonCtoA_clicked();

    void on_pushButtonCtoB_clicked();

private:

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QTimer *timer;
    QGraphicsRectItem *disk;


    int seconds;
    int moves ;
    int disk_number;
    QString player_name;

    QMap< QString, QList < QString > > highscores; //datastruckture for file
    QList < QGraphicsRectItem* > pointer_list;  //all disk pointers(smallest disk to biggest)
    //Lists to check if moves are legal.
    QList < int > towerA;
    QList < int > towerB;
    QList < int > towerC;


    const QString file_name = "highscores.txt";

    const int STEP = 20;
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 260;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 680;

    //Adds second to lcd.
    void second_gone();
    //Adds move to lcd.
    void add_move();
    //Starts game and sets labels.
    void start_game();
    //Moves disk if it is legal.
    void move_disk(QList <int> &tower_from, QList <int> &tower_to, int distance, QBrush &color);
    //Checks if move is legal.
    bool is_legal(QList <int> tower_from, QList <int> tower_to);
    //Ends the game and adds player info to listwidget and textfile.
    void won();
    void refresh_list();
    //Returns disks pointer.
    QGraphicsRectItem* get_disk(QList <int> tower);


};

#endif // MAINWINDOW_HH
