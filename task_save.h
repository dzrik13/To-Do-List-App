#ifndef TASK_SAVE_H
#define TASK_SAVE_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include "taskwidget.h"
#include <QObject>

class task_save : public QObject
{
    Q_OBJECT
public:
    QString file;
    task_save();
    task_save(QString file);
    void saveTasks(const QVector<TaskWidget*>& tasklist, const QVector<TaskWidget*>& completedList);
    void loadTasks(QVector<TaskWidget*>& taskList, QVector<TaskWidget*>& completedList);

signals:
    void setupLayouts();
};

#endif // TASK_SAVE_H
