#include "ofMain.h"
#include "j4cDAC.h"
#include "ofxIldaPoint.h"

static int num_dac_in_lan;

class ofxWinEtherdream : public ofThread
{
public:

	ofxWinEtherdream(const string _ip_addr, const int _pps) : ip_addr(_ip_addr), id(-1), pps(_pps), b_found_dac(false)
	{
		ofSleepMillis(1200);
		static bool call_once = [&]() -> bool { get_num_dac_in_lan(); return true; }();

		for (auto i = 0; i < num_dac_in_lan; i++)
		{
			auto this_addr = EtherDreamGetIP(&i);
			if (ip_addr == this_addr)
			{
				id = i;
				auto res = EtherDreamOpenDevice(&id);
				if (res)
					ofLog() << "open " << ip_addr << " dac.";
				else
					ofLogError() << "fail to open " << ip_addr << " dac.";
				b_found_dac = true;
				break;
			}
		}

		if (b_found_dac)
			startThread();
		else
			ofLogError() << "no " << ip_addr << "dac.";

		points.resize(1);
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
				if (EtherDreamGetStatus(&id) == GET_STATUS_READY)
					EtherDreamWriteFrame(&id, (EAD_Pnt_s*)points.data(), points.size() * sizeof(EAD_Pnt_s), pps, 1);
			}
			ofSleepMillis(1000 * 0.0001);
		}
	}

private:

	void get_num_dac_in_lan()
	{
		num_dac_in_lan = EtherDreamGetCardNum();
		ofLog() << "found " << num_dac_in_lan << " dacs";
	}

	string ip_addr;
	bool b_found_dac;
	int id, pps;
	vector<ofxIlda::Point> points;
};
