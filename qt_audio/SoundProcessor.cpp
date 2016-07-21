#include "SoundProcessor.h"
#include "SoundTouch.h"
#include "STTypes.h"
using namespace soundtouch;

const double SoundProcessor::CONV = 1.0 / 32768.0;

/// Convert from float to integer and saturate
inline int saturate(float fvalue, float minval, float maxval)
{
	if (fvalue > maxval) 
	{
		fvalue = maxval;
	} 
	else if (fvalue < minval)
	{
		fvalue = minval;
	}
	return (int)fvalue;
}


SoundProcessor::SoundProcessor(int channels, int bitsPerSample)
{
	_pBaseSound = new BaseSound();
	//_pBaseSound = new BaseSound();
	_channels = channels;
	_bitsPerSample = bitsPerSample / 8;
	BaseSound* soundTouch = (BaseSound*) _pBaseSound;
	soundTouch->setSampleRate(8000);
	soundTouch->setChannels(channels);
	//设置变调变速参数
	//void setRateChange(double newRate);
	//变速变调，范围为：-95 -- 5000
	//变速不变调，范围为： -95 -- 5000
	//void setTempoChange(double newTempo);
	//变调不变速，范围为：-60 -- 60
	//void setPitchSemiTones(double newPitch);
	//void setPitchSemiTones(int newPitch);
	soundTouch->setPitchSemiTones(10);
}

SoundProcessor::~SoundProcessor(void)
{
	delete _pBaseSound;
	delete _pAudioCapture;
}

void SoundProcessor::putData(void* date, unsigned int size)
{
	BaseSound* soundTouch = (BaseSound*) _pBaseSound;
	short* tmpbuff = (short*)date;
	int numElems = size / _bitsPerSample;
	for (int i = 0; i < numElems; ++i)
	{
		short value = tmpbuff[i];
		_sampleBuffer[i] = (float)(value * CONV);
	}
	int nSamples = numElems / _channels;
	soundTouch->putSamples(_sampleBuffer, nSamples);

}

unsigned int SoundProcessor::receiveData(void* date, unsigned int maxSize)
{
	BaseSound* soundTouch = (BaseSound*) _pBaseSound;
	short* tmp = (short*)date;
	int nSamples = soundTouch->receiveSamples(_sampleBuffer, maxSize / (_channels * _bitsPerSample));
	for (int i = 0; i < nSamples * _channels; ++i)
	{ 
		short value = (short)saturate(_sampleBuffer[i] * 32768.0f, -32768.0f, 32767.0f);
		tmp[i] = value;
	}

	return nSamples * _channels * _bitsPerSample;
}

// 输入缓冲区最大长度与DATA_BYTE_MAX_SIZE相同，inputSize小于等于DATA_BYTE_MAX_SIZE
// 输出数据直接写在输入缓冲区上，返回值为输出数据大小
int SoundProcessor::processedData(void* data, int inputSize)
{
	putData(data, inputSize);//把音频数据压入音频处理
	int outputSize= receiveData(data,DATA_BYTE_MAX_SIZE);//非阻塞获取音频处理后的变音数据包
	return outputSize;
}