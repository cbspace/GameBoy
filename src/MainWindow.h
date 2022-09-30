#pragma once

#include "GameBoyView.h"
#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QGridLayout>
#include <QTimer>


class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow();

    protected:
        void contextMenuEvent(QContextMenuEvent* event) override;

    private slots:
        void file();
        void open();
        void view();
        void scale1x();
        void scale2x();
        void about();

    private:
        void create_actions();
        void create_menus();

        QMenu* file_menu;
        QMenu* view_menu;
        QMenu* about_menu;
        QAction* open_act;
        QAction* scale1x_act;
        QAction* scale2x_act;

        GameBoyView gbview;
        QTimer* timer;

};