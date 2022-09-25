#include "GameBoyView.h"
#include "lib/Types.h"

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
    cout << "Hello paint" << endl;
}

QImage GameBoyView::render_gb_image() {
    QImage q_image(p_width, p_height, QImage::Format_RGB32);
    u32* buffer = emulator.disp.get_buffer();
    for (int y=0; y < p_height; y++) {
        for (int x=0; x < p_width; x++) {
            u32 pix_value = buffer[y*p_width + x];
            int r = pix_value >> 16;
            int g = (pix_value >> 8) & 0xff;
            int b = pix_value & 0x0000ff;
            QRgb value = qRgb(r,g,b);
            q_image.setPixel(x,y,value);
        }
    }
    return q_image;
}

void GameBoyView::start_emulator() {
    const QStringList args = QCoreApplication::arguments();
    emulator.start(args.at(1).toStdString(),true,false);
}