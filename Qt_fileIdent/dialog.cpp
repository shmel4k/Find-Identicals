#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    model = new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());

    ui->dirView->setModel(model);
}

Dialog::~Dialog()
{
    delete model;
    delete ui;
}


void Dialog::on_dirView_clicked(const QModelIndex &index)
{
    if (model->isDir(index))
    {
        DirName = model->fileInfo(index).absoluteFilePath();
    }
    else
    {
        DirName = model->fileInfo(index).absolutePath();
    }
}

QString Dialog::getDirName()
{
    return DirName;
}
