#pragma once

#include "GameBoyView.h"
#include <QMainWindow>
#include <QMenuBar>



namespace GBEmulator {

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow();
};

}

using GBEmulator::MainWindow;