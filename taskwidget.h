#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>

class TaskWidget : public QWidget
{
Q_OBJECT

private:
    QLabel *taskLabel;
    QLabel *taskFNotes;

    void adjustFontSize();
    // taskContainer puts the layout in a widget since scroll area will expect one.
    QVBoxLayout *tasklayout;
public:
    // Constructor
    explicit TaskWidget(const QString &taskname, const QString &taskNotes, QWidget *parent = nullptr);
    QPushButton *completeButton;
    void setLabel(const QString name)
    {
        this->taskLabel->setText(name);
    }
    QString getLabel()
    {
        return taskLabel->text();
    }
    void setNotes(const QString notes)
    {
        this->taskFNotes->setText(notes);
    }
    QString getNotes()
    {
        return taskFNotes->text();
    }

    QCheckBox *checkbox;
private slots:
    void taskCompleteClicked();

signals:
    void taskComplete(TaskWidget* taskWidget);

};

#endif // TASKWIDGET_H
