#ifndef DPSSIM_H
#define DPSSIM_H

#include <QMainWindow>

namespace Ui {
class DPSSim;
}

class DPSSim : public QMainWindow{
    Q_OBJECT

public:
    explicit DPSSim(QWidget *parent = 0);
    ~DPSSim();

private slots:
    void on_DPSmage_clicked();

    void on_pushButton_2_clicked();

    void on_DPSpriest_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::DPSSim *ui;
};

#endif // DPSSIM_H
