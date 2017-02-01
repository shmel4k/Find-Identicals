#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QFileSystemModel>
#include <QTreeView>
#include <QDir>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    QString getDirName();

private slots:
    void on_dirView_clicked(const QModelIndex &index);

private:
    Ui::Dialog *ui;
    QFileSystemModel *model;

    QString DirName;
};

#endif // DIALOG_H
