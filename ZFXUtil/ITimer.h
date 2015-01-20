#pragma once

class ITimer
{
public:
	ITimer() {}
	~ITimer() {}

	virtual void Reset() = 0;

	virtual unsigned long getMillisecondTime() = 0;
};

