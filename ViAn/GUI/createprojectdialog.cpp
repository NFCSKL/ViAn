#include "createprojectdialog.h"
#include <QSize>
#include <QPushButton>
#include <QBoxLayout>
#include <QFormLayout>
#include <QFileDialog>

#include <QDebug>

CreateProjectDialog::CreateProjectDialog(QWidget *parent) : QDialog(parent) {
    setSizeGripEnabled(false);
    QVBoxLayout* vertical_layout = new QVBoxLayout;
    path_text = new QLineEdit(this);
    name_text = new QLineEdit(this);
    QPushButton* browse_btn = new QPushButton(tr("Browse"), this);
    btn_box = new QDialogButtonBox(Qt::Horizontal);

    path_text->setFixedWidth(200);
    browse_btn->setFixedHeight(path_text->height());
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);

    QHBoxLayout* browse_layout = new QHBoxLayout;
    browse_layout->addWidget(path_text);
    browse_layout->addWidget(browse_btn);

    QFormLayout* text_btn_layout = new QFormLayout;
    text_btn_layout->addRow("Name", name_text);
    text_btn_layout->addRow("Path", browse_layout);

    vertical_layout->addLayout(text_btn_layout);
    vertical_layout->addWidget(btn_box);
    setLayout(vertical_layout);

    connect(browse_btn, &QPushButton::clicked, this, &CreateProjectDialog::browse_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &CreateProjectDialog::ok_btn_clicked);
    connect(btn_box->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &CreateProjectDialog::cancel_btn_clicked);

    show();
}

void CreateProjectDialog::browse_btn_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose project path"), path_text->text());
    if(!dir.isEmpty()) {
        path_text->setText(dir);
        qDebug() << dir;
    }
}

void CreateProjectDialog::ok_btn_clicked() {
    qDebug() << "ok";
    emit project_path(name_text->text(), path_text->text());
    close();

}

void CreateProjectDialog::cancel_btn_clicked() {
    close();
}
