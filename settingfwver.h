#ifndef SETTINGFWVER_H
#define SETTINGFWVER_H

#include <QWidget>

namespace Ui {
class SettingFwVer;
}

class SettingFwVer : public QWidget
{
    Q_OBJECT

public:
    explicit SettingFwVer(QWidget *parent = 0);
    ~SettingFwVer();
    void init();
private slots:
    void on_btn_row_clicked();

    void on_btn_lte_clicked();

    void on_btn_prc_clicked();

    void on_btn_exit_clicked();

    void on_btn_passwd_clicked();

    void on_ckb_bootloader_clicked();

    void on_btn_select_offline_fw_clicked();

private:
    Ui::SettingFwVer *ui;
};

#endif // SETTINGFWVER_H
