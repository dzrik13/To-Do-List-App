#ifndef EDIT_TASK_WINDOW_H
#define EDIT_TASK_WINDOW_H

#include <taskwidget.h>
#include <QDialog>

namespace Ui {
class edit_task_window;
}

class edit_task_window : public QDialog
{
    Q_OBJECT

public:
    explicit edit_task_window(QWidget *parent = nullptr);
    ~edit_task_window();

    void setCurrentWidget(TaskWidget *widget)
    {
        currentwidget = widget;
    }

signals:
    void edittaskData(const QString &taskName, const QString &taskNotes, TaskWidget *currentwidget);
private slots:
    void on_buttonBox_accepted();

private:
    TaskWidget *currentwidget = nullptr;
    Ui::edit_task_window *ui;
};

#endif // EDIT_TASK_WINDOW_H
