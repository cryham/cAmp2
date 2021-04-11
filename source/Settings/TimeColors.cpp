#include "TimeColors.h"
#include "../System/FileSystem.h"
#include "../System/Utilities.h"
#include "../../libs/tinyxml2.h"
using namespace std;  using namespace tinyxml2;


TimeColors::TimeColors()
{	}

TimeClrSets::TimeClrSets()
{	}
	
TimeClr TimeClrSets::Get(float t, int mode)
{
	if (cur < vt.size())
		return vt[cur].Get(t, mode);
	
	return TimeClr(1.f, 0.4f,0.7f,1.0f);
}


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

///  Load
//------------------------------------------------------------------------------------------------
bool TimeClrSets::Load()
{
	//Default();
	vt.clear();

	const string fclr = "/colors.xml";
	string file = FileSystem::Config() + fclr;
	if (!FileSystem::Exists(file))
	{
		file = FileSystem::Data() + fclr;
		if (!FileSystem::Exists(file))
			return false;
	}

	const string sErr = string("Colors Load Error: ") + file + "\n";
	ifstream fi;
	fi.open(file);
	if (!fi.good())
	{	Log(sErr + "Can't open file.");  return false;  }
	
	XMLDocument doc;
	XMLError er = doc.LoadFile(file.c_str());
	if (er != XML_SUCCESS)
	{	Log(sErr + "Can't load file.");  return false;  }

	XMLElement* root = doc.RootElement();
	if (!root)
	{	Log(sErr + "No root.");  return false;  }
	string rn = root->Name();
	if (rn != "cAmp2")
	{	Log(sErr + "Root not cAmp2.");  return false;  }

	XMLElement* n, *m;  const char* a;

	//  load
	n = root->FirstChildElement("TimeColors");
	if (!n)
	{	Log(sErr + "No <TimeColors>.");  return false;  }
	
	while (n)
	{
		TimeColors tc;
		a = n->Attribute("name");  if (a)  tc.name = string(a);

		m = n->FirstChildElement("clr");
		if (!m)  Log(sErr + "Warning, TimeColors has no <clr>" + tc.name);
		while (m)
		{
			TimeClr c(1.f, 0.2f,0.6f,0.9f);
			a = m->Attribute("time");	if (a)	c.t = (float)atof(a);
			a = m->Attribute("rgb");	if (a)	sscanf(a, "%f %f %f", &c.r, &c.g, &c.b);
			tc.vc.push_back(c);
			m = m->NextSiblingElement("clr");
		}
		vt.push_back(tc);
		n = n->NextSiblingElement("TimeColors");
		//Log("TimeColors " + tc.name + " " + i2s(tc.vc.size()));
	}

	Log("Colors Loaded.");
	return true;
}

///  Save
//------------------------------------------------------------------------------------------------
/*bool TimeClrSets::Save()
{
	return true;
}
*/
