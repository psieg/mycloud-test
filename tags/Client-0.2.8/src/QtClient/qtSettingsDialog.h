#ifndef QTSETTINGSDIALOG_H
#define QTSETTINGSDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/qmessagebox>
#include "ui_qtSettingsDialog.h"
#include "mc.h" //wether MC_WATCHMODE is defined
#include "mc_db.h"


class qtSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	qtSettingsDialog(QWidget *parent = 0);
	~qtSettingsDialog();

	enum DialogCode {
		Rejected = 0,
		Accepted = 1,
		NeedRestart = 2
	};

public slots:
	void accept();

private slots:
	void passchange();
	void acceptActivate();

private:
	Ui::qtSettingsDialog ui;
	mc_status s;
	bool passchanged;
};

#endif // QTSETTINGSDIALOG_H