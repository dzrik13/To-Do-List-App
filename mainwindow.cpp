#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskwidget.h"
#include <QMessageBox>
#include <QFileDialog>
#include "add_task_window.h"
#include "edit_task_window.h"
#include "task_save.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setStyleSheet(
        "QProgressBar {"
        "    background-color: #e0e0e0;"   // Background color of the entire progress bar
        "    border-radius: 5px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: transparent;"    // Color of the progress chunk
        "    border-radius: 5px;"
        "}");

    ui->progressBar->setMaximum(maxtasks);
    ui->progressBar->setValue(completedtasks);

    ui->progressBar->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->progressBar->setFormat("%v/%m");

    // setup for current tab
    QWidget *taskContainer = new QWidget(this);
    QHBoxLayout *overall_layout = new QHBoxLayout(this);
    overall_layout->setAlignment(Qt::AlignLeft);
    taskContainer->setLayout(overall_layout);

    ui->sArea1->setWidgetResizable(true);
    ui->sArea1->setWidget(taskContainer);

    // setup select and clear buttons
    ui->com_clear->setHidden(true);
    ui->com_selectall->setHidden(true);
    ui->cur_clear->setHidden(true);
    ui->cur_selectall->setHidden(true);

    // setup for completed tab
    QWidget *taskContainer2 = new QWidget(this);
    QHBoxLayout *overall_layout2 = new QHBoxLayout(this);
    overall_layout2->setAlignment(Qt::AlignLeft);
    taskContainer2->setLayout(overall_layout2);

    ui->sArea2->setWidgetResizable(true);
    ui->sArea2->setWidget(taskContainer2);

    savefile = new task_save();
    connect(savefile, &task_save::setupLayouts, this, &MainWindow::fix_layouts);
    savefile->loadTasks(taskmemaddress, completedmemaddress);
}

MainWindow::~MainWindow()
{
    savefile->saveTasks(taskmemaddress,completedmemaddress);

    delete ui;
    delete savefile;
}

void MainWindow::on_addTask_clicked()
{
    Add_task_window addwindow;
    connect(&addwindow, &Add_task_window::addtaskData, this, &MainWindow::addTaskDataConfirmed);

    addwindow.setModal(true);
    addwindow.exec();
}

void MainWindow::addTask(const QString &taskName, const QString &taskNotes)
{
    ui->progressBar->setStyleSheet(
        "QProgressBar {"
        "    background-color: #e0e0e0;"   // Background color of the entire progress bar
        "    border-radius: 5px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #45bf53;"    // Color of the progress chunk
        "    border-radius: 5px;"
        "}");

    TaskWidget *taskWidget = new TaskWidget(taskName, taskNotes, this);
    taskWidget->setStyleSheet("background-color: rgb(237,237,237);");
    connect(taskWidget, &TaskWidget::taskComplete, this, &MainWindow::handleCompletion);

    if(ui->deleteTask->text() == "Confirm")
    {
        taskWidget->checkbox->setHidden(false);
    }

    taskmemaddress.push_back(taskWidget);

    QWidget *container = ui->sArea1->widget();
    QHBoxLayout *layout = static_cast<QHBoxLayout*>(container->layout());

    layout->addWidget(taskWidget);

    maxtasks++;
    ui->progressBar->setMaximum(maxtasks);
}

void MainWindow::handleCompletion(TaskWidget* currentWidget)
{
    // grab layouts to add the widgets to
    QWidget *currentcontainer = ui->sArea1->widget();
    QHBoxLayout *currentlayout = static_cast<QHBoxLayout*>(currentcontainer->layout());

    QWidget *completedcontainer = ui->sArea2->widget();
    QHBoxLayout *completedlayout = static_cast<QHBoxLayout*>(completedcontainer->layout());


    if(currentWidget->completeButton->text() == "Complete")
    {
        // move current widget to complete widget
        currentlayout->removeWidget(currentWidget);
        completedlayout->addWidget(currentWidget);

        completedmemaddress.push_back(currentWidget);

        completedtasks++;
        ui->progressBar->setValue(completedtasks);

        currentWidget->completeButton->setText("Move Back");
    }
    else
    {
        completedlayout->removeWidget(currentWidget);
        currentlayout->addWidget(currentWidget);

        completedmemaddress.removeAll(currentWidget);

        completedtasks--;
        ui->progressBar->setValue(completedtasks);

        currentWidget->completeButton->setText("Complete");
    }

}

void MainWindow::addTaskDataConfirmed(const QString &taskName, const QString &taskNotes)
{
    if (!taskName.isEmpty()) {
        addTask(taskName, taskNotes);
    } else {
        QMessageBox::warning(this, "Warning", "Task name cannot be empty!");
    }
}

void MainWindow::on_deleteTask_clicked()
{
    ui->com_clear->setHidden(false);
    ui->com_selectall->setHidden(false);
    ui->cur_clear->setHidden(false);
    ui->cur_selectall->setHidden(false);

    if(taskmemaddress.isEmpty() == true) return;

    if(ui->deleteTask->text() == "Delete Task")
    {
        setAllWidgetCheckboxesHidden(false);
        ui->deleteTask->setText("Confirm");
    }
    else
    {
        for(int i = taskmemaddress.size() - 1; i >= 0; i--)
        {
            widgetptr = taskmemaddress[i];
            if(completedmemaddress.contains(widgetptr) && widgetptr->checkbox->isChecked())
            {
                completedmemaddress.removeAll(widgetptr);
                completedtasks--;
                ui->progressBar->setValue(completedtasks);
            }
            if(widgetptr->checkbox->isChecked())
            {
                widgetptr->deleteLater();
                taskmemaddress.removeAt(i);
                maxtasks--;
                ui->progressBar->setMaximum(maxtasks);
            }
        }
        ui->deleteTask->setText("Delete Task");

        setAllWidgetCheckboxesHidden(true);

        if(maxtasks == 0)
        {
            ui->progressBar->setStyleSheet(
                "QProgressBar {"
                "    background-color: #e0e0e0;"   // Background color of the entire progress bar
                "    border-radius: 5px;"
                "}"
                "QProgressBar::chunk {"
                "    background-color: transparent;"    // Color of the progress chunk
                "    border-radius: 5px;"
                "}");
        }

        ui->com_clear->setHidden(true);
        ui->com_selectall->setHidden(true);
        ui->cur_clear->setHidden(true);
        ui->cur_selectall->setHidden(true);
    }

}

void MainWindow::on_editTask_clicked()
{
    if(taskmemaddress.isEmpty() == true) return;

    if(ui->editTask->text() == "Edit Task")
    {
        ui->com_clear->setHidden(false);
        ui->com_selectall->setHidden(false);
        ui->cur_clear->setHidden(false);
        ui->cur_selectall->setHidden(false);

        setAllWidgetCheckboxesHidden(false);
        ui->editTask->setText("Confirm Selection");
    }
    else
    {
        edit_task_window editwindow;
        connect(&editwindow, &edit_task_window::edittaskData, this, &MainWindow::editTaskDataConfirmed);

        editwindow.setModal(true);
        for(int i = 0; i < taskmemaddress.size(); i++)
        {
            if(taskmemaddress[i]->checkbox->isChecked())
            {
                editwindow.setCurrentWidget(taskmemaddress[i]);
                editwindow.exec();
            }
        }

        for(int i = taskmemaddress.size() - 1; i >= 0; i--)
        {
            widgetptr = taskmemaddress[i];
            widgetptr->checkbox->setChecked(false);
        }

        ui->editTask->setText("Edit Task");
        setAllWidgetCheckboxesHidden(true);

        ui->com_clear->setHidden(true);
        ui->com_selectall->setHidden(true);
        ui->cur_clear->setHidden(true);
        ui->cur_selectall->setHidden(true);
    }
}

void MainWindow::editTaskDataConfirmed(const QString &taskName, const QString &taskNotes, TaskWidget *currentwidget)
{
    if (!taskName.isEmpty()) {
        currentwidget->setLabel(taskName);
        currentwidget->setNotes(taskNotes);
    } else {
        QMessageBox::warning(this, "Warning", "Task name cannot be empty!");
    }
}

void MainWindow::setAllWidgetCheckboxesHidden(bool choice)
{
    for(int i = taskmemaddress.size() - 1; i >= 0; i--)
    {
        widgetptr = taskmemaddress[i];
        widgetptr->checkbox->setHidden(choice);
    }
}

void MainWindow::setAllWidgetCheckboxesChecked(bool choice)
{
    for(int i = taskmemaddress.size() - 1; i >= 0; i--)
    {
        widgetptr = taskmemaddress[i];
        widgetptr->checkbox->setChecked(choice);
    }
}

void MainWindow::fix_layouts()
{
    // grab layouts to add the widgets to
    QWidget *currentcontainer = ui->sArea1->widget();
    QHBoxLayout *currentlayout = static_cast<QHBoxLayout*>(currentcontainer->layout());

    QWidget *completedcontainer = ui->sArea2->widget();
    QHBoxLayout *completedlayout = static_cast<QHBoxLayout*>(completedcontainer->layout());

    foreach (TaskWidget* task, taskmemaddress)
    {
        if(ui->deleteTask->text() == "Confirm")
        {
            task->checkbox->setHidden(false);
        }

        if(completedmemaddress.contains(task))
        {
            completedlayout->addWidget(task);
        }
        else if(taskmemaddress.contains(task))
        {
            currentlayout->addWidget(task);
        }

        task->setStyleSheet("background-color: rgb(237,237,237);");
        connect(task, &TaskWidget::taskComplete, this, &MainWindow::handleCompletion);
    }

    maxtasks = taskmemaddress.size();
    completedtasks = completedmemaddress.size();
    ui->progressBar->setMaximum(maxtasks);
    ui->progressBar->setValue(completedtasks);

    if(maxtasks == 0 && completedtasks == 0)
    {
        ui->progressBar->setStyleSheet(
            "QProgressBar {"
            "    background-color: #e0e0e0;"   // Background color of the entire progress bar
            "    border-radius: 5px;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: transparent;"    // Color of the progress chunk
            "    border-radius: 5px;"
            "}");
    }
    else
    {
        ui->progressBar->setStyleSheet(
            "QProgressBar {"
            "    background-color: #e0e0e0;"   // Background color of the entire progress bar
            "    border-radius: 5px;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: #45bf53;"    // Color of the progress chunk
            "    border-radius: 5px;"
            "}");
    }
}

void MainWindow::on_cur_selectall_clicked()
{
    for(int i = taskmemaddress.size() - 1; i >= 0; i--)
    {
        widgetptr = taskmemaddress[i];
        if(!completedmemaddress.contains(widgetptr))
        {
            widgetptr->checkbox->setChecked(true);
        }
    }
}


void MainWindow::on_com_selectall_clicked()
{
    for(int i = taskmemaddress.size() - 1; i >= 0; i--)
    {
        widgetptr = taskmemaddress[i];
        if(completedmemaddress.contains(widgetptr))
        {
            widgetptr->checkbox->setChecked(true);
        }
    }
}


void MainWindow::on_com_clear_clicked()
{
    for(int i = taskmemaddress.size() - 1; i >= 0; i--)
    {
        widgetptr = taskmemaddress[i];
        if(completedmemaddress.contains(widgetptr))
        {
            widgetptr->checkbox->setChecked(false);
        }
    }
}


void MainWindow::on_cur_clear_clicked()
{
    for(int i = taskmemaddress.size() - 1; i >= 0; i--)
    {
        widgetptr = taskmemaddress[i];
        if(!completedmemaddress.contains(widgetptr))
        {
            widgetptr->checkbox->setChecked(false);
        }
    }
}

void MainWindow::on_actionImport_Save_triggered()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, "Import JSON File", "", "JSON Files (*.json);;All Files (*)");

    if (filename.isEmpty())
    {
        return;
    }

    //clear current screen
    setAllWidgetCheckboxesChecked(true);

    for(int i = taskmemaddress.size() - 1; i >= 0; i--)
    {
        widgetptr = taskmemaddress[i];
        if(widgetptr->checkbox->isChecked())
        {
            widgetptr->deleteLater();
        }
    }
    completedmemaddress.clear();
    taskmemaddress.clear();
    completedtasks = 0;
    ui->progressBar->setValue(completedtasks);
    maxtasks = 0;
    ui->progressBar->setMaximum(maxtasks);

    task_save loadimport(filename);
    connect(&loadimport, &task_save::setupLayouts, this, &MainWindow::fix_layouts);
    loadimport.loadTasks(taskmemaddress, completedmemaddress);
}


void MainWindow::on_actionExport_Save_triggered()
{
    QString filename = QFileDialog::getSaveFileName(nullptr, "Export JSON File", "", "JSON Files (*.json);;All Files (*)");
    if(filename.isEmpty())
    {
        QMessageBox::information(nullptr, "No File Name", "Please provide a valid file name");
        return;
    }
    if(!filename.endsWith(".json"))
    {
        filename += ".json";
    }

    task_save saveimport(filename);
    saveimport.saveTasks(taskmemaddress,completedmemaddress);
}

void MainWindow::clearLayout(QLayout *layout)
{
    if (layout == NULL)
        return;
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::about(nullptr, "Help", "To add a task, simply press the add task button in the top left corner and input a name and any additional notes you want. \n\n"
                                        "When you edit or delete a task you will first be able to select multiple tasks by checking them in their top-right corners.\n\n"
                                        "Your tasks are automatically saved and loaded when you open and close the program.\n\nTo import and export saves, press file in the top-left corner.");
}


void MainWindow::on_actionAbout_this_application_triggered()
{
    QMessageBox::about(nullptr, "About this application", "This is a simple to-do list application designed to help you organize your daily tasks into one place. Created by dzrik.");
}

