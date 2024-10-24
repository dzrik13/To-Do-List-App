#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <taskwidget.h>
#include <task_save.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int completedtasks = 0;
    int maxtasks = 0;
    QVector<TaskWidget *> taskmemaddress;
    QVector<TaskWidget *> completedmemaddress;

private slots:
    void on_addTask_clicked();
    void handleCompletion(TaskWidget *currentWidget);
    void addTaskDataConfirmed(const QString &taskName, const QString &taskNotes);
    void editTaskDataConfirmed(const QString &taskName, const QString &taskNotes, TaskWidget *currentwidget);
    void on_deleteTask_clicked();
    void on_editTask_clicked();
    void fix_layouts();
    void clearLayout(QLayout *layout);

    void on_cur_selectall_clicked();

    void on_com_selectall_clicked();

    void on_com_clear_clicked();

    void on_cur_clear_clicked();

    void on_actionImport_Save_triggered();

    void on_actionExport_Save_triggered();

    void on_actionHelp_triggered();

    void on_actionAbout_this_application_triggered();

private:
    Ui::MainWindow *ui;
    task_save *savefile;
    void addTask(const QString &taskname, const QString &taskNotes);
    void setAllWidgetCheckboxesHidden(bool choice);
    void setAllWidgetCheckboxesChecked(bool choice);
    TaskWidget *widgetptr = nullptr;
};
#endif // MAINWINDOW_H
