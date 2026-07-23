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

#pragma once

#include <QString>

class QWidget;

namespace pv {
namespace dialogs {

class CaptureSetupDlg
{
public:
    CaptureSetupDlg();
    ~CaptureSetupDlg();

    bool ShowDlg(QWidget *parent);

private:
    QString _saveDir;
};

} // namespace dialogs
} // namespace pv