#pragma once

#include "displayconst.h"
#include "System.h"
#include "lib/Types.h"
#include "lib/Error.h"
#include "MainWindow.h"
#include <optional>
#include <QCoreApplication>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class MainWindow;

class GameBoyView : public QWidget {
    Q_OBJECT

    public:
        GameBoyView(MainWindow* parent);
        ~GameBoyView();
        void parse_command_line();
        void start_emulator(string rom_path, bool boot_rom, bool debug);
        void set_scaling_factor(u8 sf);

        System system;

    public slots:
        void animate();

    protected:
        void paintEvent(QPaintEvent* event) override;
    
    private:
        MainWindow* m_parent { nullptr };
        QImage render_gb_image();
        i32 width;
        i32 height;
        u8 scaling_factor = 2;

};