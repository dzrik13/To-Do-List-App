#include "task_save.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

task_save::task_save()
{
    file = "taskdata.json";
}

task_save::task_save(QString imp_file)
{
    file = imp_file;
}

void task_save::saveTasks(const QVector<TaskWidget*>& tasklist, const QVector<TaskWidget*>& completedList)
{
    QFile saveFile(file);

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonArray taskarray;
    foreach (TaskWidget* task, tasklist)
    {
        QJsonObject taskObject;
        taskObject["label"] = task->getLabel();
        taskObject["notes"] = task->getNotes();
        if(completedList.contains(task))
        {
            taskObject["isComplete"] = true;
        }
        else
        {
            taskObject["isComplete"] = false;
        }
        taskarray.append(taskObject);
    }

    QJsonDocument saveDoc(taskarray);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

void task_save::loadTasks(QVector<TaskWidget*>& taskList, QVector<TaskWidget*>& completedList)
{
    QFile loadFile(file);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't load file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    loadFile.close();

    QJsonArray taskarray = loadDoc.array();
    for(int i = 0; i < taskarray.size(); i++)
    {
        QJsonObject taskObject = taskarray[i].toObject();
        QString taskName = taskObject["label"].toString();
        QString taskNotes = taskObject["notes"].toString();

        TaskWidget* task = new TaskWidget(taskName, taskNotes, nullptr);
        taskList.append(task);
        if(taskObject["isComplete"] == true)
        {
            completedList.append(task);
            task->completeButton->setText("Move Back");
        }
    }

    emit setupLayouts();
}
