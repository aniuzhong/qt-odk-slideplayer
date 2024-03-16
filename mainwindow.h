#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SlideShowController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void doLoad(const char* url);

private slots:
    void on_pushButton_load_clicked();
    void on_pushButton_start_clicked();
    void on_pushButton_gotoNextEffect_clicked();
    void on_pushButton_gotoPreviousEffect_clicked();
    void on_pushButton_gotoNextSlide_clicked();
    void on_pushButton_gotoPreviousSlide_clicked();
    void on_pushButton_gotoFirstSlide_clicked();
    void on_pushButton_gotoLastSlide_clicked();
    void on_pushButton_getSlideCount_clicked();

    void on_pushButton_stopSound_clicked();

private:
    Ui::MainWindow*      ui;
    SlideShowController* m_slideShowController;
};

#endif // MAINWINDOW_H
