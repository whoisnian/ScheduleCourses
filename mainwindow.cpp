#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hopcroft-karp.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QListWidget>
#include <QTableWidget>
#include <QStatusBar>
#include <QFile>
#include <QDebug>

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

void MainWindow::on_action_import_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择要导入的文件", "/");
    if(fileName == NULL)
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    while(!file.atEnd())
    {
        QString line = file.readLine();
        if(line.split(QRegExp("\\s+"))[0] == QString("---"))
        {
            line = file.readLine();
            teachers.push_back(line.split(QRegExp("\\s+"))[0].toStdString());
            std::vector<std::string> *t = new std::vector<std::string>;
            courses.push_back(t);
            ui->listWidget_teachers->addItem(line.split(QRegExp("\\s+"))[0]);
        }
        else
        {
            courses.back()->push_back(line.split(QRegExp("\\s+"))[0].toStdString());
            courseslist.insert(line.split(QRegExp("\\s+"))[0].toStdString());
        }
    }
    file.close();
}

void MainWindow::on_action_exit_triggered()
{
    this->close();
}

void MainWindow::on_pushButton_add_teacher_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "教师", "教师名称：", QLineEdit::Normal, "", &ok);
    if(!ok) return;
    teachers.push_back(name.toStdString());
    std::vector<std::string> *t = new std::vector<std::string>;
    courses.push_back(t);
    ui->listWidget_teachers->addItem(name);
    ui->tableWidget_result->clear();
    ui->tableWidget_result->setRowCount(0);
    ui->statusBar->clearMessage();
}

void MainWindow::on_pushButton_add_course_clicked()
{
    int row = ui->listWidget_teachers->currentRow() ;
    if(row == -1)
    {
        QMessageBox::information(this, "缺少目标", "请在右侧教师窗口中选择要给哪位教师设置课程", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    QInputDialog *input = new QInputDialog(this);
    input->setWindowTitle(QString("课程"));
    input->setLabelText(QString("课程名称："));
    input->setComboBoxEditable(true);
    QStringList completer;
    for(std::set<std::string>::iterator it = courseslist.begin();it != courseslist.end();it++)
        completer.push_back(QString(it->c_str()));
    input->setComboBoxItems(completer);
    bool ok = input->exec();
    if(!ok) return;
    courses.at(row)->push_back(input->textValue().toStdString());
    courseslist.insert(input->textValue().toStdString());
    ui->listWidget_courses->addItem(input->textValue());
    ui->tableWidget_result->clear();
    ui->tableWidget_result->setRowCount(0);
    ui->statusBar->clearMessage();
}

void MainWindow::on_pushButton_delete_clicked()
{
    int row_teacher = ui->listWidget_teachers->currentRow();
    if(row_teacher == -1)
    {
        QMessageBox::information(this, "缺少目标", "请在右侧窗口中选择要删除的条目", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    int row_course = ui->listWidget_courses->currentRow();
    if(row_course == -1)
    {
        teachers.erase(teachers.begin() + row_teacher);
        courses.erase(courses.begin() + row_teacher);
        ui->listWidget_teachers->takeItem(row_teacher);
    }
    else
    {
        courses.at(row_teacher)->erase(courses.at(row_teacher)->begin() + row_course);
        ui->listWidget_courses->takeItem(row_course);
    }
    ui->tableWidget_result->clear();
    ui->tableWidget_result->setRowCount(0);
    ui->statusBar->clearMessage();
}

void MainWindow::on_pushButton_clear_clicked()
{
    teachers.clear();
    courses.clear();
    ui->listWidget_teachers->clear();
    ui->listWidget_courses->clear();
    ui->tableWidget_result->clear();
    ui->tableWidget_result->setRowCount(0);
    ui->statusBar->clearMessage();
}

void MainWindow::on_pushButton_run_clicked()
{
    for(int i = 0;i < int(courses.size());i++)
    {
        if((*(courses.begin() + i))->size() <= 0)
        {
            QMessageBox::information(this, "教师课程不完整", QString("请完善 ") + QString(teachers.at(i).c_str()) + QString(" 的课程"), QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
    }
    HopcroftKarp graph(courses, courseslist);
    ui->statusBar->showMessage(QString("最大匹配数：") + QString::number(graph.maxmatch(), 10));
    std::vector<std::string> temp;
    for(std::set<std::string>::iterator it = courseslist.begin();it != courseslist.end();it++)
        temp.push_back(*it);
    int *result = graph.result();
    for(int i = 0;i < int(teachers.size());i++)
    {
        if(result[i] < 0) continue;
        ui->tableWidget_result->setRowCount(ui->tableWidget_result->rowCount() + 1);
        QTableWidgetItem *Teacher = new QTableWidgetItem(QString(teachers.at(i).c_str()));
        QTableWidgetItem *Course = new QTableWidgetItem(QString(temp.at(result[i]).c_str()));
        ui->tableWidget_result->setItem(i, 0, Teacher);
        ui->tableWidget_result->setItem(i, 1, Course);
    }
}

void MainWindow::on_listWidget_teachers_activated()
{
    ui->listWidget_courses->clear();
    for(std::vector<std::string>::iterator it = courses.at(ui->listWidget_teachers->currentRow())->begin();it != courses.at(ui->listWidget_teachers->currentRow())->end();it++)
        ui->listWidget_courses->addItem(QString(it->c_str()));
}
