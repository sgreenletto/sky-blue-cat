#ifndef PETWIDGET_H
#define PETWIDGET_H

#include <QWidget>
#include "petgifwindow.h"

class PetWidget : public QWidget {
    Q_OBJECT
public:
    PetWidget(QWidget *parent = nullptr);
    ~PetWidget();
};

#endif // PETWIDGET_H

