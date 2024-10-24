#include "taskwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSize>

TaskWidget::TaskWidget(const QString &taskName, const QString &taskNotes, QWidget *parent) : QWidget(parent)
{
    tasklayout = new QVBoxLayout(this);

    // task name setup
    taskLabel = new QLabel(taskName, this);
    QFont font("Segoe UI", 20);
    taskLabel->setFont(font);
    taskLabel->setAlignment(Qt::AlignCenter);
    taskLabel->setWordWrap(true);
    tasklayout->addWidget(taskLabel);

    //check box setup
    checkbox = new QCheckBox(this);
    checkbox->setParent(this);
    checkbox->setGeometry(100 + 80,2, 20, 20);
    checkbox->setHidden(true);

    // task notes setup
    taskFNotes = new QLabel(taskNotes, this);
    taskFNotes->setWordWrap(true);
    taskFNotes->setIndent(10);
    taskFNotes->setAlignment(Qt::AlignTop);
    taskFNotes->setFixedHeight(150);
    tasklayout->addWidget(taskFNotes);


    // completed button setup
    completeButton = new QPushButton("Complete", this);
    tasklayout->addWidget(completeButton);
    connect(completeButton, &QPushButton::clicked, this, &TaskWidget::taskCompleteClicked);

    tasklayout->setContentsMargins(0, 0, 0, 0); // No margins
    tasklayout->setSpacing(0); // No spacing between widgets


    setFixedSize(200, 200);
    setLayout(tasklayout);

    adjustFontSize();
}

void TaskWidget::adjustFontSize()
{
    QFont font = taskLabel->font();
    if (taskLabel->text().size() >= 30)
    {
        font.setPointSize(10);
    }
    else if (taskLabel->text().size() >= 20)
    { // change this value as needed
        font.setPointSize(12);
    }
    else if (taskLabel->text().size() >= 10)
    { // change this value as needed
        font.setPointSize(15);
    }
    else
    {
        font.setPointSize(20);
    }

    taskLabel->setFont(font);
}

void TaskWidget::taskCompleteClicked()
{
    emit taskComplete(this);
}
