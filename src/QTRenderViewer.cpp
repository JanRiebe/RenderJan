#include <QApplication>


int main(int argc, char *argv[])
{
     QApplication *application = new QApplication(argc, argv);
     QWidget *widget = new QWidget();
     widget->setFixedSize(500, 500);
     widget->show();
     QLabel *label = new QLabel(widget);
     label->setGeometry(10, 10, 480, 480);
     label->show();
     QByteArray *temp = new QByteArray();
     QFile *file = new QFile("image.png");
     *temp = file->readAll();
     QPixmap *pix = new QPixmap();
     pix->loadFromData(*temp);
     label->setPixmap(*pix);
     return application->exec();
}
