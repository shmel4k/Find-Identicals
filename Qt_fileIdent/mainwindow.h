#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>

#include "dialog.h"

#include <iostream>
#include <algorithm>

#include <string>
#include <vector>
#include <unordered_set>

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>


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
    void on_startButton_clicked();

    void on_SelectDirButton1_clicked();

    void on_SelectDirButton2_clicked();

private:
    Ui::MainWindow *ui;

    QStringListModel *model;
    QString dir1,dir2;

    Dialog *tree;

    QStringList Analyze(QString, QString);
    bool isEqual(std::string, std::string);
};

#endif // MAINWINDOW_H
