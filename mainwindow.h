#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <set>

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
    void on_action_import_triggered();

    void on_action_exit_triggered();

    void on_pushButton_add_teacher_clicked();

    void on_pushButton_add_course_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_run_clicked();

    void on_listWidget_teachers_activated();

private:
    Ui::MainWindow *ui;
    std::vector<std::string> teachers; //教师名字
    std::map<unsigned long long, int> teachers_used; //已出现过的教师名字哈希值，用来快速判重
    std::vector< std::vector<std::string> *> courses; //每一个教师对应的课程名字
    std::map< std::pair<std::string, std::string>, int> courses_used; //每一对教师与课程的对应组合，用来快速判重
    std::set<std::string> courseslist;
};

#endif // MAINWINDOW_H
