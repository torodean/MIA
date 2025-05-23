#ifndef D3CWINDOW_H
#define D3CWINDOW_H

#include <QMainWindow>
#include <string>

namespace Ui {
class D3CWindow;
}

class D3CWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit D3CWindow(QWidget *parent = 0);
    void hideWindowItems();
    void showMassConversion();
    void hideMassConversion();
    void showHome();
    void hideHome();
    void showEncryption();
    void hideEncryption();
    void showEncryptionPW();
    void hideEncryptionPW();
    void showAdvanced();
    void hideAdvanced();
    void showHelp();
    void hideHelp();
    void showChangeLog();
    void hideChangeLog();
    void showConversionUnits();
    void hideConversionUnits();
    void showKinematics1D();
    void hideKinematics1D();
    void primePopulateProgressBar(int percent);
    bool convertMultipleEntries();
    ~D3CWindow();

private slots:
    void on_actionHome_triggered();
    void on_actionExit_triggered();
    void on_actionD0s_triggered();
    void on_actionD0s1_triggered();
    void on_actionD3C_prompt_triggered();
    void on_D3CPromptEnter_clicked();
    void on_encryptButton_clicked();
    void on_decryptButton_clicked();
    void on_actionAvailable_Functions_triggered();
    void on_actionMass_triggered();
    void on_actionChange_Log_triggered();
    void on_cryptResetButton_clicked();
    void on_D3CPromptLineEnter_returnPressed();
    void on_convertMassReset_clicked();
    void on_massConvertButton_clicked();
    void on_actionConversion_Units_triggered();
    void on_action1D_Kinematics_triggered();
    void on_D3CPromptReset_clicked();
    void on_actionHide_Windows_triggered();

    void on_Kinematics1DResetButton_clicked();

    void on_encryptButtonPW_clicked();

    void on_cryptResetButtonPW_clicked();

    void on_decryptButtonPW_clicked();

private:
    Ui::D3CWindow *ui;
};

#endif // D3CWINDOW_H
