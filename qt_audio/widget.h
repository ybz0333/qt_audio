#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAudioInput>
#include <QAudioOutput>
//#include <QFile>
#include "SoundProcessor.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    QAudioInput *mpAudioInputSound;
    QAudioOutput *mpAudioOutputSound;

    QIODevice *mpInputDevSound;
    QIODevice *mpOutputDevSound;

	SoundProcessor soundProcessor;

private slots:
    void OnReadMore();
};

#endif // WIDGET_H
