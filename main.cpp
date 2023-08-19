#include "MainView.h"

#include "Scene.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char** argv){
    QApplication app(argc, argv);

    Scene scene;
    MainView view;
    view.setScene(&scene);
    view.show();

    return app.exec();
}
