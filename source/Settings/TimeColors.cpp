#include "TimeColors.h"
using namespace std;


TimeColors::TimeColors()
{	}


//  get color for time, interpolate
TimeClr TimeColors::Get(float ti, int mode)
{
	int l = (int)vc.size();
	if (l > 1 && mode != 0)
	{
		float t = ti/60.f;
		int i1 = 0, i2 = 0;
		for (int i=1; i < l; ++i)
			if (t > vc[i-1].t)	{	i1=i2;  i2=i;	}
		
		if (mode==2)  // 2 linear smooth
		{
			float t2 = vc[i2].t, t1 = vc[i1].t,
				d = t2 - t1, v1,v2;
			if (d > 0.f)  {  v2 = (t - t1) / d;  v1 = (t2 - t) / d;  }
			else  {  v2 = 1.f;  v1 = 0.f;  }
			
			return TimeClr(1.f,
				vc[i1].r * v1 + vc[i2].r * v2,
				vc[i1].g * v1 + vc[i2].g * v2,
				vc[i1].b * v1 + vc[i2].b * v2);
		}
		else  // 1 last
			return TimeClr(1.f, vc[i2].r, vc[i2].g, vc[i2].b);
	}
	else  // 0 const
		return TimeClr(1.f, 0.4f,0.7f,1.0f);
}
