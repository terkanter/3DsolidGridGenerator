
#include <QGuiApplication>
#include "model/model.h"
#include <QQuickView>

class QuickViewEdited: public QQuickView {
public:
    QuickViewEdited():
        QQuickView() {}
public:
    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::Close) {
            // your code here
        }
        return QQuickView::event(event);
    }
};
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Model *model = new Model();
    model->separation();
    qDebug() << "Ready to Transf " << model->exportToTransf();;
    return 0;
}
