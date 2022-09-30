#pragma once

#include "displayconst.h"
#include "Emulator.h"
#include <QCoreApplication>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class GameBoyView : public QWidget {
    Q_OBJECT

    public:
        GameBoyView(QWidget* parent, int width_initial, int height_initial);
        ~GameBoyView();
        void start_emulator();

    public slots:
        void animate();

    protected:
        void paintEvent(QPaintEvent* event) override;
    
    private:
        Emulator emulator;
        QImage render_gb_image();
        QBrush background;
        int p_width;
        int p_height;

};