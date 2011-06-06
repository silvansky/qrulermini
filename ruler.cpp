#include "ruler.h"
#include "ui_ruler.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>

Ruler::Ruler(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Ruler)
{
	ui->setupUi(this);
	alpha = 255;
	setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
	co = Qt::Vertical;
	relayout();
}

Ruler::~Ruler()
{
	delete ui;
}

void Ruler::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void Ruler::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		dragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
}

void Ruler::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		move(event->globalPos() - dragPosition);
		event->accept();
	}
}

void Ruler::keyPressEvent(QKeyEvent * ke)
{
	if (ke->key() == Qt::Key_Escape)
		qApp->quit();
}

void Ruler::paintEvent(QPaintEvent * pe)
{
	QPainter p(this);
	p.setClipRect(pe->rect());
	QSize sz = size();
	// bg
	p.fillRect(QRect(QPoint(0, 0), sz), QColor(0, 0, 0, alpha));
	// lines
	static QList<int> intervals = QList<int>() << 2 << 3 << 5 << 10 << 15 << 18 << 20 << 30 << 40 << 50 << 1;
	int x = 0;
	p.setPen(QPen(QColor::fromRgb(255, 0, 0)));
	QFont f = p.font();
	f.setPixelSize(9);
	p.setFont(f);
	//p.set
	foreach (int i, intervals)
	{
		if (co == Qt::Horizontal)
		{
			p.drawLine(QPoint(x, 23), QPoint(x, sz.height()));
			if (i >= 10)
			{
				// rotated text
				QRect textRect = QRect(x, 23, i, sz.height() - 23);
				p.save();
				QTransform t;
				t.translate(textRect.left() + textRect.width() / 2, textRect.top() + textRect.height() / 2);
				t.rotate(90);
				p.setTransform(t);
				textRect.setWidth(sz.height() - 23);
				textRect.moveLeft(textRect.left() + 1);
				textRect.setHeight(i);
				p.drawText(textRect.translated(-textRect.left() - textRect.width() / 2, -textRect.top() - textRect.height() / 2), QString("%1px").arg(i), QTextOption(Qt::AlignCenter));
				p.restore();
			}
		}
		else
		{
			p.drawLine(QPoint(23, x), QPoint(sz.width(), x));
			if (i >= 10)
			{
				// text
				QRect textRect = QRect(23, x, sz.width() - 23, i);
				p.drawText(textRect, QString("%1px").arg(i), QTextOption(Qt::AlignCenter));
			}
		}
		x += i - 1;
	}
}


Qt::Orientation Ruler::orientation() const
{
	return co;
}

void Ruler::setOrientation(Qt::Orientation newOrientation)
{
	co = newOrientation;
	QRect oldGeom = geometry();
	QRect newGeom = QRect(oldGeom.topLeft(), QSize(oldGeom.height(), oldGeom.width()));
	setGeometry(newGeom);
	relayout();
}

void Ruler::relayout()
{
	QBoxLayout * newLayout;
	QBoxLayout * parentLayout;
	if (co == Qt::Vertical)
	{
		newLayout = new QVBoxLayout();
		parentLayout = new QHBoxLayout(this);
	}
	else
	{
		newLayout = new QHBoxLayout();
		parentLayout = new QVBoxLayout(this);
	}
	newLayout->setContentsMargins(0, 0, 0, 0);
	parentLayout->setContentsMargins(2, 2, 2, 2);
	newLayout->addWidget(ui->toolButton);
	ui->verticalSlider->setOrientation(co);
	newLayout->addWidget(ui->verticalSlider);
	newLayout->addStretch();
	if (layout())
		layout()->deleteLater();
	parentLayout->addLayout(newLayout);
	parentLayout->addStretch();
	setLayout(parentLayout);
}

void Ruler::on_toolButton_toggled(bool checked)
{
	setOrientation(checked ? Qt::Horizontal : Qt::Vertical);
}

void Ruler::on_verticalSlider_valueChanged(int value)
{
	alpha = value;
	update();
}
