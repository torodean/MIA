#define WINVER 0x0500
#include <windows.h>
#include "dpssim.h"
#include "ui_dpssim.h"
#include "Mage.h"

DPSSim::DPSSim(QWidget *parent) : QMainWindow(parent), ui(new Ui::DPSSim){
    ui->setupUi(this);
}

DPSSim::~DPSSim(){
    delete ui;
}

Mage spell;

void DPSSim::on_DPSmage_clicked(){
    Sleep(5000);
    spell.openingRotation();
    spell.normalRotation();
}

void DPSSim::on_pushButton_2_clicked(){
    spell.setNormalRotationRun(false);
    spell.setOpeningRotationRun(false);
}

void DPSSim::on_DPSpriest_clicked()
{
    for(int i=0;i<1000;i++){
        spell.castFrostBolt();
    }
}

void DPSSim::on_pushButton_clicked()
{
    for(int i=0;i<1000;i++){
        Sleep(1000);
        spell.castFrostfireBolt();
    }
}

void DPSSim::on_pushButton_3_clicked()
{
    for(int i=0;i<10000;i++){
        spell.tab();
        spell.castFrostBolt();
        if(i%2 == 0){
            spell.right();
        }else{
            spell.left();
        }
    }
}

void DPSSim::on_pushButton_4_clicked()
{
    for(int i=0;i<1000;i++){
        spell.castFrostBolt();
    }
}

void DPSSim::on_pushButton_5_clicked()
{
    Sleep(5000);
    for(int i=0;i<10;i++){
        spell.petguru();
    }
}

void DPSSim::on_pushButton_6_clicked()
{
    Sleep(5000);
    for(int i=0;i<500;i++){
        spell.castFrostBolt();
        spell.leftclick();
    }
}
