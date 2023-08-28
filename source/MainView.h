#pragma once

#include <QGraphicsView>

class MainView : public QGraphicsView
{
    Q_OBJECT

public:
    MainView(QWidget* parent = nullptr);


private:

    void resizeEvent(QResizeEvent* event) override;

};
