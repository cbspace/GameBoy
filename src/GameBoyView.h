#pragma once

#include "displayconst.h"
#include "Emulator.h"
#include "lib/Types.h"
#include <QCoreApplication>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class GameBoyView : public QWidget {
    Q_OBJECT

    public:
        GameBoyView(QWidget* parent);
        ~GameBoyView();
        void start_emulator();
        void set_scaling_factor(u8 sf);

    public slots:
        void animate();

    protected:
        void paintEvent(QPaintEvent* event) override;
    
    private:
        QWidget* m_parent { NULL };
        Emulator emulator;
        QImage render_gb_image();
        i32 width;
        i32 height;
        u8 scaling_factor = 3;
};