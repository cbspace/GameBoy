#pragma once

#include "displayconst.h"
#include "Emulator.h"
#include <QCoreApplication>
#include <QAbstractScrollArea>
#include <QPaintEvent>
#include <QPainter>

class GameBoyView : public QAbstractScrollArea {
    Q_OBJECT

    public:
        GameBoyView(int width_initial, int height_initial);
        virtual ~GameBoyView() override;
        virtual void paintEvent(QPaintEvent* event) override;
        void start_emulator();

        Emulator emulator;
    
    private:
        QImage render_gb_image();
        int p_width;
        int p_height;

};