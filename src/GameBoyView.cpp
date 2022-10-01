#include "GameBoyView.h"
#include "lib/Types.h"

GameBoyView::GameBoyView(QWidget* parent) :
    QWidget(parent),
    m_parent(parent)
    {
        width = DISP_W * scaling_factor;
        height = DISP_H * scaling_factor;
        setFixedSize(width, height);
    }

GameBoyView::~GameBoyView() {}

void GameBoyView::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);
    
    QImage q_image = render_gb_image();
    painter.drawImage(QPoint(0,0), q_image);

    painter.end();
}

QImage GameBoyView::render_gb_image() {
    QImage q_image(width, height, QImage::Format_RGB32);
    u32* buffer = emulator.disp.get_buffer();
    for (int y=0; y < DISP_H; y++) {
        for (int x=0; x < DISP_W; x++) {
            u32 pix_value = buffer[y*DISP_W + x];
            int r = pix_value >> 16;
            int g = (pix_value >> 8) & 0xff;
            int b = pix_value & 0x0000ff;
            QRgb value = qRgb(r,g,b);

            for (u16 y1 = 0; y1 < scaling_factor; y1++)
            {
                for (u16 x1 = 0; x1 < scaling_factor; x1++)
                {
                    q_image.setPixel(x * scaling_factor + x1, y * scaling_factor + y1, value);
                }
            }
        }
    }
    return q_image;
}

void GameBoyView::start_emulator() {
    const QStringList args = QCoreApplication::arguments();
    emulator.start(args.at(1).toStdString(),true,false);
}

void GameBoyView::animate() {
    bool new_frame_drawn = false;
    while (!new_frame_drawn) {
        emulator.main_loop();
        new_frame_drawn = emulator.disp.new_frame_is_drawn();
        update();
    }
}

void GameBoyView::set_scaling_factor(u8 sf) {
    scaling_factor = sf;
    width = DISP_W * scaling_factor;
    height = DISP_H * scaling_factor;
    setFixedSize(width, height);
    m_parent->adjustSize();
}