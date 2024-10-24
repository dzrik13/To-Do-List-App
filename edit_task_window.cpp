#include "edit_task_window.h"
#include "ui_edit_task_window.h"

edit_task_window::edit_task_window(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::edit_task_window)
{
    ui->setupUi(this);
    ui->textEdit->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

edit_task_window::~edit_task_window()
{
    delete ui;
}

void edit_task_window::on_buttonBox_accepted()
{
    QString text = ui->textEdit->toPlainText();
    emit edittaskData(ui->lineEdit->text(), text, currentwidget);
}

