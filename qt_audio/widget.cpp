#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

#define BufferSize 14096

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QAudioFormat mFormatSound;
    // set up the format you want, eg.
    mFormatSound.setSampleRate(8000);
    mFormatSound.setChannelCount(1);
    mFormatSound.setSampleSize(16);
    mFormatSound.setCodec("audio/pcm");
    mFormatSound.setByteOrder(QAudioFormat::LittleEndian);
    mFormatSound.setSampleType(QAudioFormat::UnSignedInt);
    //mFormatSound.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());
    if (!infoIn.isFormatSupported(mFormatSound))
    {
        //Default format not supported - trying to use nearest
        mFormatSound = infoIn.nearestFormat(mFormatSound);
    }

    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());
    if (!infoOut.isFormatSupported(mFormatSound))
    {
        //Default format not supported - trying to use nearest
        mFormatSound = infoOut.nearestFormat(mFormatSound);
    }

    QAudioDeviceInfo inputDevice(QAudioDeviceInfo::defaultInputDevice());
    mpAudioInputSound = new QAudioInput(inputDevice, mFormatSound, this);

    QAudioDeviceInfo outputDevice(QAudioDeviceInfo::defaultOutputDevice());
    mpAudioOutputSound = new QAudioOutput(outputDevice, mFormatSound, this);

    mpOutputDevSound = mpAudioOutputSound->start();
    mpInputDevSound = mpAudioInputSound->start();
    connect(mpInputDevSound, SIGNAL(readyRead()), SLOT(OnReadMore()));

    qDebug() <<"record begin!" << endl;
}

void Widget::OnReadMore()
{
    //Return if audio input is null
    if(!mpAudioInputSound)
        return;

    QByteArray _Buffer(BufferSize, 0);
    //Check the number of samples in input buffer
    qint64 len = mpAudioInputSound->bytesReady();
    qDebug() << "length1 = " << len;
    //Limit sample size
    if(len > 4096)
        len = 4096;

    //Read sound samples from input device to buffer
    qint64 l = mpInputDevSound->read(_Buffer.data(), len);
    qDebug() << "length2 = " << l;
    if(l > 0) {
        //mpOutputDevSound->write(_Buffer, len);
		int outputSize = soundProcessor.processedData(_Buffer.data(), l);
		mpOutputDevSound->write(_Buffer, outputSize);
    }
}

Widget::~Widget()
{
    mpAudioInputSound->stop();
    delete mpAudioInputSound;
    mpAudioOutputSound->stop();
    delete mpAudioOutputSound;

    delete ui;
}
