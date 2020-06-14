#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:



    void on_cleanTables_clicked();



    void on_actionRun_triggered();

    void on_actionRead_triggered();

    void on_actionSave_triggered();

    void on_actionClean_triggered();

    void on_actionExit_triggered();


    void on_actionInformaci_n_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
