/*
#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
*/

#include "SoundProcessor.h"
#include <iostream>
using namespace std;
//using namespace soundtouch;

void main() {
	char gcTemp[4096];
	memset(gcTemp,0,4096);
	SoundProcessor soundProcessor;
	soundProcessor.processedData(gcTemp, 4096);
	system("pause");
}