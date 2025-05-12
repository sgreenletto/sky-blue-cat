#include "petwidget.h"

PetWidget::PetWidget(QWidget *parent)
    : QWidget(parent)
{
    PetGifWindow *mainPet = new PetGifWindow(Idle, nullptr);
    mainPet->show();
}

PetWidget::~PetWidget() {}