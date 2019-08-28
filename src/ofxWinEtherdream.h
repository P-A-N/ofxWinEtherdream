#include "ofMain.h"
#include "j4cDAC.h"
#include "ofxIldaPoint.h"

class ofxWinEtherdream : public ofThread
{
public:

	ofxWinEtherdream(const int _id, const int _pps) : id(_id), pps(_pps)
	{
		ofSleepMillis(1200);
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
		waitForThread(true);
	}

	void set_points(const vector<ofxIlda::Point>& _points)
	{
		if (!_points.empty())
			points = _points;
	}

	void clear() { points.clear(); }

protected:

	virtual void threadedFunction() override
	{
		while (isThreadRunning())
		{
			if (!points.empty())
			{
				EtherDreamWriteFrame(&id, (EAD_Pnt_s*)points.data(), points.size() * sizeof(EAD_Pnt_s), pps, 1);
			}
		}
	}

private:

	int id, pps;
	vector<ofxIlda::Point> points;
};
