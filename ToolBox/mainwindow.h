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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    //void on_pushButton_2_clicked();

    void on_loadBtn_clicked();

    void on_saveBtn_clicked();

    void on_convertBtn_clicked();

    void on_loadBtn_2_clicked();

    void on_saveBtn_2_clicked();

    void on_convertBtn_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
