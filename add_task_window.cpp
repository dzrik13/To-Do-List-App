#include "add_task_window.h"
#include "ui_add_task_window.h"

Add_task_window::Add_task_window(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Add_task_window)
{
    ui->setupUi(this);
    ui->textEdit->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

Add_task_window::~Add_task_window()
{
    delete ui;
}

void Add_task_window::on_buttonBox_accepted()
{
    QString text = ui->textEdit->toPlainText();
    emit addtaskData(ui->lineEdit->text(), text);
}

