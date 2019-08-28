#include "ofMain.h"
#include "j4cDAC.h"
#include "ofxIldaPoint.h"

class ofxWinEtherdream : public ofThread
{
public:

	ofxWinEtherdream(const int _id, const int _pps) : id(_id), pps(_pps)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));
		auto res = EtherDreamOpenDevice(&id);
		if (res)
			ofLog() << "open " << id << " dac.";
		else
			ofLogError() << "fail to open " << id << " dac.";
		startThread();
	}

	~ofxWinEtherdream()
	{
		if (id != -1) 
		{
			EtherDreamStop(&id);
			EtherDreamCloseDevice(&id);
		}
		EtherDreamClose();
		std::unique_lock<std::mutex> lck(mutex);
		stopThread();
		condition.notify_all();
		waitForThread(true);
	}

	void set_points(const vector<ofxIlda::Point>& _points)
	{
		if (!_points.empty())
		{
			std::unique_lock<std::mutex> lock(mutex);
			points = std::move(_points);
			condition.notify_all();
		}
	}

protected:

	virtual void threadedFunction() override
	{
		while (isThreadRunning())
		{
			if (!points.empty())
			{
				std::unique_lock<std::mutex> lock(mutex);
				EtherDreamWriteFrame(&id, (EAD_Pnt_s*)points.data(), points.size() * sizeof(EAD_Pnt_s), pps, 1);
				condition.wait(lock);
				ofSleepMillis(1000 * 0.1);
			}
		}

	}

private:

	std::condition_variable condition;
	int id, pps;
	vector<ofxIlda::Point> points;
};