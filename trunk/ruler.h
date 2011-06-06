#ifndef RULER_H
#define RULER_H

#include <QWidget>

namespace Ui {
class Ruler;
}

class Ruler : public QWidget
{
	Q_OBJECT

public:
	explicit Ruler(QWidget *parent = 0);
	~Ruler();

protected:
	void changeEvent(QEvent *e);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void keyPressEvent(QKeyEvent *);
	void paintEvent(QPaintEvent *);
	Qt::Orientation orientation() const;
	void setOrientation(Qt::Orientation newOrientation);
	void relayout();
private slots:
	void on_toolButton_toggled(bool checked);
	void on_verticalSlider_valueChanged(int value);
private:
	Ui::Ruler *ui;
	int alpha;
	Qt::Orientation co;
	QPoint dragPosition;
};

#endif // RULER_H
