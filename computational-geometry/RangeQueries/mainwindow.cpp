//
// GUI-template for CG-assignments
//
// (c) Georg Umlauf, 2014
// (c) Georg Umlauf, 2020: Qt5
// (c) Georg Umlauf, 2022: Qt6
//

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass) {
    ui->setupUi(this);
    ui->glwidget->setFocusPolicy(Qt::StrongFocus);
    QObject::connect(ui->showPartitionButton, &QPushButton::toggled,
                     ui->glwidget, &GLWidget::showPartition);
    QObject::connect(ui->useInsertButton, &QPushButton::toggled, ui->glwidget,
                     &GLWidget::useInsert);
    QObject::connect(ui->showDotButton, &QPushButton::pressed, ui->glwidget,
                     &GLWidget::showDot);
}

MainWindow::~MainWindow() { delete ui; }
