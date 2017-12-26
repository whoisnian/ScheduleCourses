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

unsigned long long counthash(std::string st)
{
    int len = st.length(), i;
    unsigned long long hash = 0;
    for(i = 0;i < len;i++)
        hash = hash * 131 + st[i];
    return hash;
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
            if(teachers_used[counthash(line.split(QRegExp("\\s+"))[0].toStdString())] > 0)
            {
                QMessageBox::information(this, "重复项", "包含重复的教师项，导入失败", QMessageBox::Ok, QMessageBox::Ok);
                return;
            }
            teachers_used[counthash(line.split(QRegExp("\\s+"))[0].toStdString())] = 1;
            teachers.push_back(line.split(QRegExp("\\s+"))[0].toStdString());
            std::vector<std::string> *t = new std::vector<std::string>;
            courses.push_back(t);
            ui->listWidget_teachers->addItem(line.split(QRegExp("\\s+"))[0]);
        }
        else
        {
            if(courses_used[std::pair<std::string, std::string>(teachers.back(), line.split(QRegExp("\\s+"))[0].toStdString())] > 0)
            {
                QMessageBox::information(this, "重复项", "教师包含重复的课程项，导入失败", QMessageBox::Ok, QMessageBox::Ok);
                return;
            }
            courses_used[std::pair<std::string, std::string>(teachers.back(), line.split(QRegExp("\\s+"))[0].toStdString())] = 1;
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
    if(teachers_used[counthash(name.toStdString())] > 0)
    {
        QMessageBox::information(this, "重复项", "该教师已添加过", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    teachers_used[counthash(name.toStdString())] = 1;
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
    if(courses_used[std::pair<std::string, std::string>(teachers.at(ui->listWidget_teachers->currentRow()), input->textValue().toStdString())] > 0)
    {
        QMessageBox::information(this, "重复项", "该教师的该课程已添加过", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    courses_used[std::pair<std::string, std::string>(teachers.at(ui->listWidget_teachers->currentRow()), input->textValue().toStdString())] = 1;
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
        teachers_used[counthash(teachers.at(ui->listWidget_teachers->currentRow()))] = 0;
        teachers.erase(teachers.begin() + row_teacher);
        courses.erase(courses.begin() + row_teacher);
        ui->listWidget_teachers->takeItem(row_teacher);
        ui->listWidget_courses->clear();
    }
    else
    {
        courses_used[std::pair<std::string, std::string>(teachers.at(ui->listWidget_teachers->currentRow()), courses.at(row_teacher)->at(row_course))] = 0;
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
    teachers_used.clear();
    courses_used.clear();
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
    ui->tableWidget_result->setRowCount(0);
    for(int i = 0;i < int(teachers.size());i++)
    {
        if(result[i] < 0) continue;
        ui->tableWidget_result->setRowCount(ui->tableWidget_result->rowCount() + 1);
        QTableWidgetItem *Teacher = new QTableWidgetItem(QString(teachers.at(i).c_str()));
        QTableWidgetItem *Course = new QTableWidgetItem(QString(temp.at(result[i]).c_str()));
        ui->tableWidget_result->setItem(ui->tableWidget_result->rowCount() - 1, 0, Teacher);
        ui->tableWidget_result->setItem(ui->tableWidget_result->rowCount() - 1, 1, Course);
    }
}

void MainWindow::on_listWidget_teachers_activated()
{
    ui->listWidget_courses->clear();
    for(std::vector<std::string>::iterator it = courses.at(ui->listWidget_teachers->currentRow())->begin();it != courses.at(ui->listWidget_teachers->currentRow())->end();it++)
        ui->listWidget_courses->addItem(QString(it->c_str()));
}
