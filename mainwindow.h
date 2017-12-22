#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <vector>
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
    std::vector<std::string> teachers;
    std::vector< std::vector<std::string> *> courses;
    std::set<std::string> courseslist;
};

#endif // MAINWINDOW_H
