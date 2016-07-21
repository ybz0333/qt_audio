#ifndef _SOUNDPROCESSOR_H_
#define _SOUNDPROCESSOR_H_
//class BaseSound;
class SoundProcessor
{
public:
	SoundProcessor(int channels = 1, int bitsPerSample = 16);
	virtual ~SoundProcessor(void);
	int processedData(void* data, int inputSize);
protected:
private:
	static const int DATA_BYTE_MAX_SIZE = 14096;//定义数据包的最大大小
	static const int SAMPLE_MAX_SIZE = DATA_BYTE_MAX_SIZE / 2;

	void *_pBaseSound;
	void *_pAudioCapture;
	int   _channels;
	int   _bitsPerSample;
	float _sampleBuffer[SAMPLE_MAX_SIZE];

	static const double CONV;

	void putData(void* date, unsigned int size);
	unsigned int receiveData(void* date, unsigned int maxSize);
};

#endif // _SOUNDPROCESSOR_H_