#include <cassert>

#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>

#include "slideshowcontroller.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    assert(this->winId() != ui->documentWidget->winId());

    m_slideShowController = new SlideShowController;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_slideShowController;
}

void MainWindow::doLoad(const char* url)
{
    m_slideShowController->load(url, this->ui->documentWidget->winId());
}

void MainWindow::on_pushButton_load_clicked()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        doLoad(fileDialog.selectedUrls().front().toString().toStdString().c_str());
}

void MainWindow::on_pushButton_start_clicked()
{
    m_slideShowController->start();
}

void MainWindow::on_pushButton_gotoNextEffect_clicked()
{
    m_slideShowController->gotoNextEffect();
}

void MainWindow::on_pushButton_gotoPreviousEffect_clicked()
{
    m_slideShowController->gotoPreviousEffect();
}

void MainWindow::on_pushButton_gotoNextSlide_clicked()
{
    m_slideShowController->gotoNextSlide();
}

void MainWindow::on_pushButton_gotoPreviousSlide_clicked()
{
    m_slideShowController->gotoPreviousSlide();
}

void MainWindow::on_pushButton_gotoFirstSlide_clicked()
{
    m_slideShowController->gotoFirstSlide();
}

void MainWindow::on_pushButton_gotoLastSlide_clicked()
{
    m_slideShowController->gotoLastSlide();
}

void MainWindow::on_pushButton_getSlideCount_clicked()
{
    int slideCount = m_slideShowController->getSlideCount();
    ui->plainTextEdit->appendPlainText("getSlideCount:" + QString::number(slideCount));
}


void MainWindow::on_pushButton_stopSound_clicked()
{
    m_slideShowController->stopSound();
}

