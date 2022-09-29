#include "GameBoyView.h"
#include "lib/Types.h"

GameBoyView::GameBoyView(QWidget* parent, int width_initial, int height_initial) :
    QWidget(parent),
    p_width(width_initial), 
    p_height(height_initial)
    {
        setFixedSize(width_initial, height_initial);

        background = QBrush(QColor(0x92, 0xad, 0x26));
    }

GameBoyView::~GameBoyView() {}

void GameBoyView::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);
    
    painter.fillRect(event->rect(), background);

    painter.end();
}

void GameBoyView::start_emulator() {
    const QStringList args = QCoreApplication::arguments();
    emulator.start(args.at(1).toStdString(),true,false);
}

void GameBoyView::animate() {
    update();
}