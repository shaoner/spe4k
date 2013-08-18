#include "color-label.hh"

#include "ui_color-label.h"
#include <QColorDialog>
#include <QMouseEvent>
#include "common/parameters.hh"

ColorLabel::ColorLabel(int num, QWidget* parent) :
    QLabel(parent),
    _ui(new Ui::ColorLabel),
    _code(num),
    _color(QColor(Parameters::get()->ircColor(num)))
{
    _ui->setupUi(this);
    setText(((num < 10) ? "0" : "") + QString::number(num));
    setPalette(QPalette(_color));
}

ColorLabel::~ColorLabel()
{
    delete _ui;
}

void
ColorLabel::mouseReleaseEvent(QMouseEvent* event)
{
    if ((event->button() & Qt::LeftButton) == Qt::LeftButton)
    {
        QColor color = QColorDialog::getColor(_color, this);
        setPalette(QPalette(color));
        Parameters::get()->set_ircColor(_code, color.name());
    }
}
