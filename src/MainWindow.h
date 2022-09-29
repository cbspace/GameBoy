#pragma once

#include "GameBoyView.h"
#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QGridLayout>
#include <QTimer>


namespace GBEmulator {

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow();

    private:
        GameBoyView gbview;
        QTimer* timer;
        QTimer* timer2;
};

}

using GBEmulator::MainWindow;