#pragma once

#include "GameBoyView.h"
#include <QCoreApplication>
#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QGridLayout>
#include <QTimer>
#include <QFileDialog>
#include <fstream>
#include <iostream>

class GameBoyView;
class DebugWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow();
        void start_timer();
        void stop_timer();

    public slots:
        void open_debug_window();

    protected:
        void contextMenuEvent(QContextMenuEvent* event) override;

    private slots:
        void open_file();
        void stop();
        void about();

    private:
        void create_actions();
        void create_menus();

        QStatusBar* m_statusbar;

        QMenu* emulator_menu;
        QAction* open_file_act;
        QAction* open_debug_act;
        QAction* stop_act;
        QAction* quit_act;

        QMenu* view_menu;
        QAction* scale1x_act;
        QAction* scale2x_act;
        QAction* scale3x_act;
        QAction* scale4x_act;

        QMenu* about_menu;
        QAction* about_emuboy_act;
        QWidget* about_widget { nullptr };
        
        GameBoyView* gbview;
        QTimer* timer;

};