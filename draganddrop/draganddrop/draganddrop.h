#ifndef DRAGANDDROP_H
#define DRAGANDDROP_H

#include <QtWidgets/QMainWindow>
#include "ui_draganddrop.h"

class draganddrop : public QMainWindow
{
	Q_OBJECT

public:
	draganddrop(QWidget *parent = 0);
	~draganddrop();

private:
	Ui::draganddropClass ui;
};

#endif // DRAGANDDROP_H
