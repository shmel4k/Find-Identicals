#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "ui_dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(new QStringListModel),
    tree(new Dialog)
{
    ui->setupUi(this);
    model->setParent(this);

 //   QObject::connect(tree, SIGNAL(finished(int)), this, SLOT(dialogIsFinished(int)));
}

MainWindow::~MainWindow()
{
    delete tree;
    delete model;
    delete ui;
}


void MainWindow::on_SelectDirButton1_clicked()
{
    if (tree->exec() == QDialog::Accepted)
    {
        dir1 = tree->getDirName();
    }
}

void MainWindow::on_SelectDirButton2_clicked()
{
    if (tree->exec() == QDialog::Accepted)
    {
        dir2 = tree->getDirName();
    }
}

void MainWindow::on_startButton_clicked()
{
    QStringList List;

    List = Analyze(dir1, dir2);

    model->setStringList(List);
    ui->listView->setModel(model);
}

QStringList MainWindow::Analyze(QString DirName1, QString DirName2)
{
    QStringList List;

    std::vector < std::string > Dirs( {DirName1.toStdString(), DirName2.toStdString()} );

    std::unordered_set< std::string > StartList; // start list of files (in both dir),
                                                 // easy to remove elements from

    std::vector< std::unordered_set<std::string> > FinalGroups;

    try
    {
        boost::filesystem::path p;

        for (auto &dir : Dirs)
        {
            p = dir;
            if (boost::filesystem::exists(p))
            {
                if (boost::filesystem::is_directory(p))
                {
                    for (auto &x : boost::filesystem::directory_iterator(p))
                    {
                        if (boost::filesystem::is_regular_file(x.path()) &&
                            boost::filesystem::file_size(x.path()) > 0 )
                        {
                            // TODO:
                            // for optimization reasons it may be useful to remember
                            // file size as well not to retrieve it again later;
                            // to do that, algorithm has to be modified a bit

                            StartList.insert(x.path().string());
                        }
                    }
                }
            }
        }

        std::unordered_set< std::string > Dupes; // list of identical files


        for (auto x = StartList.cbegin(); x != StartList.cend(); )
        {
            Dupes.clear();
            for (auto y = std::next(x); y != StartList.cend(); )
            {
                // TODO:
                // for optimization reason it could be more effective
                // to anchor content of *x while in inner cycle not
                // to map it in isEqual() each compare pass

                if (boost::filesystem::file_size(*x) == boost::filesystem::file_size(*y)
                    && isEqual(*x, *y)) // identical files found
                {
                    Dupes.insert(*y);
                    y = StartList.erase(y);
                }
                else
                {
                    y++;
                }
            }

            if (!Dupes.empty())
            {
                Dupes.insert(*x); // base file
                x = StartList.erase(x);
                FinalGroups.push_back(Dupes);
            }
            else
            {
                x++;
            }
        }
    }

    catch (const boost::filesystem::filesystem_error& ex)
    {
//        std::cout << ex.what() << std::endl;
    }

    auto i = FinalGroups.size();
    if (0 == i)
    {
        List << "No identical files found";
    }
    else
    {        
        List << QStringLiteral("Found %1 group(s) of identical files:").arg(i);
        
        for(auto &x: FinalGroups)
        {
            List << "--------";
            for(auto y : x)
            {
                List << QString::fromUtf8(y.c_str());
            }
        }
    }    

    return List;
}

bool MainWindow::isEqual(std::string lFile, std::string rFile)
{
    boost::iostreams::mapped_file_source f1(lFile);
    boost::iostreams::mapped_file_source f2(rFile);

    return f1.size() == f2.size() && std::equal(f1.data(), f1.data() + f1.size(), f2.data());
}
