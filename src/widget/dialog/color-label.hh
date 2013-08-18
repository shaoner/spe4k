#ifndef COLORLABEL_HH
#define COLORLABEL_HH

#include <QLabel>

namespace Ui
{
	class ColorLabel;
} // namespace Ui

class ColorLabel : public QLabel
{

	Q_OBJECT

public:
	explicit ColorLabel(int num, QWidget* parent = 0);
	~ColorLabel();
protected:
	void mouseReleaseEvent(QMouseEvent* event);
private:
	Ui::ColorLabel* _ui;
	int _code;
	QColor _color;

};

#endif // COLORLABEL_HH
