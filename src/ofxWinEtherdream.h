#include "ofMain.h"
#include "j4cDAC.h"
#include "ofxIldaPoint.h"

class ofxWinEtherdream : public ofThread
{
public:

	ofxWinEtherdream(const string _ip_addr, const int _pps) : ip_addr(_ip_addr), id(-1), pps(_pps), b_found_dac(false)
	{
		ofSleepMillis(1200);
		auto cnt = EtherDreamGetCardNum();
		ofLogVerbose() << "found " << cnt << "dacs";

		for (auto i = 0; i < cnt; i++)
		{
			auto this_addr = EtherDreamGetIP(&i);
			ofLogVerbose() << "id " << i << " is " << this_addr;
			if (ip_addr == this_addr)
			{
				id = i;
				auto res = EtherDreamOpenDevice(&id);
				if (res)
					ofLog() << "open " << ip_addr << " dac.";
				else
					ofLogError() << "fail to open " << ip_addr << " dac.";
				b_found_dac = true;
			}
		}

		if (b_found_dac)
			startThread();
		else
			ofLogError() << "no " << ip_addr << "dac.";
	}

	~ofxWinEtherdream()
	{
		if (!b_found_dac) return;

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
		if (!b_found_dac) return;

		if (!_points.empty())
			points = _points;
	}

	void clear() 
	{
		if (!b_found_dac) return;

		points.clear(); 
	}

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

	string ip_addr;
	bool b_found_dac;
	int id, pps;
	vector<ofxIlda::Point> points;
};
