/*
 * This file is part of the DSView project.
 * DSView is based on PulseView.
 *
 * Copyright (C) 2026 DreamSourceLab <support@dreamsourcelab.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "capturesetupdlg.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QLineEdit>
#include <QPushButton>

#include "dsdialog.h"
#include "../appcontrol.h"
#include "../config/appconfig.h"
#include "../ui/msgbox.h"

namespace pv {
namespace dialogs {

CaptureSetupDlg::CaptureSetupDlg()
{
}

CaptureSetupDlg::~CaptureSetupDlg()
{
}

bool CaptureSetupDlg::ShowDlg(QWidget *parent)
{
    DSDialog dlg(parent, true, true);
    dlg.setTitle(QStringLiteral("Capture Setup"));
    dlg.setMinimumSize(420, 180);

    AppConfig &app = AppConfig::Instance();

    QVBoxLayout *root = new QVBoxLayout();
    root->setContentsMargins(0, 10, 0, 20);
    root->setSpacing(10);

    QCheckBox *ckAutoSave = new QCheckBox();
    ckAutoSave->setChecked(app.appOptions.autoSaveCapture);
    ckAutoSave->setText(QStringLiteral("Auto-save captured data after acquisition"));

    QLineEdit *dirEdit = new QLineEdit();
    dirEdit->setText(app.userHistory.saveDir);

    QPushButton *browseBtn = new QPushButton(QStringLiteral("Browse..."));

    QWidget *dirRow = new QWidget();
    QHBoxLayout *dirLay = new QHBoxLayout();
    dirLay->setContentsMargins(0, 0, 0, 0);
    dirLay->setSpacing(8);
    dirRow->setLayout(dirLay);
    dirLay->addWidget(dirEdit);
    dirLay->addWidget(browseBtn);

    QFormLayout *form = new QFormLayout();
    form->setContentsMargins(10, 0, 10, 0);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);
    form->addRow(QStringLiteral("Enable"), ckAutoSave);
    form->addRow(QStringLiteral("Save directory"), dirRow);

    root->addLayout(form);
    dlg.layout()->addLayout(root);

    QObject::connect(browseBtn, &QPushButton::clicked, [&]() {
        QString startDir = dirEdit->text().trimmed();
        if (startDir.isEmpty()) {
            startDir = app.userHistory.saveDir;
        }
        if (startDir.isEmpty()) {
            startDir = QDir::homePath();
        }

        QString selected = QFileDialog::getExistingDirectory(
            &dlg,
            QStringLiteral("Select Save Directory"),
            startDir);

        if (!selected.isEmpty()) {
            dirEdit->setText(selected);
        }
    });

    dlg.exec();
    if (!dlg.IsClickYes()) {
        return false;
    }

    bool changed = false;

    if (app.appOptions.autoSaveCapture != ckAutoSave->isChecked()) {
        app.appOptions.autoSaveCapture = ckAutoSave->isChecked();
        changed = true;
    }

    QString newDir = dirEdit->text().trimmed();
    if (newDir != app.userHistory.saveDir) {
        app.userHistory.saveDir = newDir;
        app.SaveHistory();
    }

    if (changed) {
        app.SaveApp();
    }

    return true;
}

} // namespace dialogs
} // namespace pv