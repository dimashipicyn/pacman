#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>

class Pacman;
class Sprite;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject* parent = nullptr);

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

    void update();

private:
    QTimer* timer_ = nullptr;
    QElapsedTimer elapsed_timer_;
    Pacman* pacman_;
    float delta_time_sec_ = 0;
};