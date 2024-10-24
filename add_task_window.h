#ifndef ADD_TASK_WINDOW_H
#define ADD_TASK_WINDOW_H

#include <QDialog>

namespace Ui {
class Add_task_window;
}

class Add_task_window : public QDialog
{
    Q_OBJECT

public:
    explicit Add_task_window(QWidget *parent = nullptr);
    ~Add_task_window();

signals:
    void addtaskData(const QString &taskName, const QString &taskNotes);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::Add_task_window *ui;
};

#endif // ADD_TASK_WINDOW_H
