#include "GameBoyView.h"

GameBoyView::GameBoyView(int width_initial, int height_initial) :
    p_width(width_initial), 
    p_height(height_initial) 
    {}

GameBoyView::~GameBoyView() {}

void GameBoyView::paintEvent(QPaintEvent* event) {
    QPainter painter(viewport());
    painter.setClipRect(event->rect());

    QImage q_image = render_gb_image();
    painter.drawImage(QPoint(0,0), q_image);
}

void GameBoyView::resizeEvent(QResizeEvent* event) {
    p_width = event->size().width();
    p_height = event->size().height();
}

QImage GameBoyView::render_gb_image() {
    QImage q_image(p_width, p_height, QImage::Format_RGB32);
    QRgb value = qRgb(0x92,0xAD,0x26);
    for (int y=0; y < p_height; y++) {
        for (int x=0; x < p_width; x++) {
            q_image.setPixel(x,y,value);
        }
    }
    return q_image;
}