#include "Themes.h"
#include "../System/FileSystem.h"
#include "../System/Utilities.h"
#include "../../libs/tinyxml2.h"
using namespace std;  using namespace tinyxml2;


Themes::Themes()
{
	//Defaults();
}


TimeClr Themes::GetTimeClr(float time, int mode)
{
	if (curTime < vTime.size())
		return vTime[curTime].Get(time, mode);
	
	return TimeClr(1.f, 0.4f,0.7f,1.0f);
}

int Themes::GetVisId(const string& name)
{
	int id = mapVisual[name];
	if (id != 0)
		return id-1;
	return 0;  // 1st, default
}

int Themes::GetFontId(const string& name)
{
	int id = mapFonts[name];
	if (id != 0)
		return id-1;
	return 0;  // 1st, default
}



void Themes::Defaults()
{
	//  add 1 working default to each vector
	//  time
	vTime.clear();
	TimeColors tc;  tc.name = "default";
	tc.vc.push_back(TimeClr(0.0f, 0.16f, 0.52f, 1.00f));
	tc.vc.push_back(TimeClr(900.0f, 0.86f, 0.92f, 1.00f));
	vTime.emplace_back(tc);

	//  visual
	vVisual.clear();
	mapVisual.clear();
	
	VisualColors c;  c.name = "default";
	c.add.h = 0.594f;  c.add.s = 0.893f;  c.add.v = 0.633f;
	c.mul.h =-0.099f;  c.mul.s =-1.033f;  c.mul.v = 0.273f;
	c.pow.h = 1.035f;  c.pow.s = 1.654f;  c.pow.v = 0.997f;
	vVisual.emplace_back(c);
	mapVisual[c.name] = vVisual.size();

	//  fonts
	vFonts.clear();
	mapFonts.clear();
	
	SFontSet fs;
	vFonts.emplace_back(fs);
	mapFonts[fs.name] = vFonts.size();
}


///  Load
//------------------------------------------------------------------------------------------------
bool Themes::Load()
{
	Defaults();

	string file = FileSystem::ThemesUser();
	if (!FileSystem::Exists(file))
	{
		file = FileSystem::Themes();
		if (!FileSystem::Exists(file))
			return false;
	}

	const string sErr = string("Themes Load Error: ") + file + "\n";
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

	
	//  load  time
	n = root->FirstChildElement("TimeColors");
	if (!n)
	{	Log(sErr + "No <TimeColors>.");  return false;  }
	
	while (n)
	{
		TimeColors tc;
		a = n->Attribute("name");  if (a)  tc.name = string(a);
		a = n->Attribute("default");  if (a)  curTime = vTime.size();

		if (tc.name != "default")
		{
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
			vTime.push_back(tc);
		}
		n = n->NextSiblingElement("TimeColors");
	}
	Log("TimeColors loaded: " + i2s(vTime.size()));

	
	//  load  visual
	n = root->FirstChildElement("VisualColors");
	if (!n)
	{	Log(sErr + "No <VisualColors>.");  /*return false;*/  }
	
	int i = 0;
	while (n)
	{
		VisualColors vc;
		a = n->Attribute("name");  if (a)  vc.name = string(a);
		if (vc.name != "default")
		{
			m = n->FirstChildElement("hsv");
			if (!m)  Log(sErr + "Warning, VisualColors has no <hsv>" + vc.name);
			while (m)
			{
				Float3HSV hsv;
				a = m->Attribute("h");	if (a)	hsv.h = s2f(a);
				a = m->Attribute("s");	if (a)	hsv.s = s2f(a);
				a = m->Attribute("v");	if (a)	hsv.v = s2f(a);
	
				a = m->Attribute("type");	int type = a ? atoi(a) : 0;
				switch (type)
				{
				case 0:  vc.add = hsv;  break;
				case 1:  vc.mul = hsv;  break;
				case 2:  vc.pow = hsv;  break;
				}
				m = m->NextSiblingElement("hsv");
			}
			vVisual.emplace_back(vc);
			mapVisual[vc.name] = vVisual.size();
		}
		a = n->Attribute("curFFT");  if (a)  curFFT = i;
		a = n->Attribute("curSpect");  if (a)  curSpect = i;
		
		n = n->NextSiblingElement("VisualColors");  ++i;
	}
	Log("VisualColors loaded: " + i2s(vVisual.size()));

	//  load  fonts
	n = root->FirstChildElement("FontSet");
	if (!n)
	{	Log(sErr + "No <FontSet>.");  /*return false;*/  }
	
	while (n)
	{
		SFontSet fs;
		a = n->Attribute("name");  if (a)  fs.name = string(a);
		if (fs.name != "default")
		{
			int i = 0;
			m = n->FirstChildElement("font");
			if (!m)  Log(sErr + "Warning, VisualColors has no <font>" + fs.name);
			while (m)
			{
				auto& f = fs.fnt[i];
				a = m->Attribute("name");	if (a)	f.name = string(a);
				a = m->Attribute("size");	if (a)	f.size = s2i(a);
				a = m->Attribute("lspc");	if (a)	f.lineSpacing = s2i(a);
				a = m->Attribute("bold");	if (a)	f.bold = s2b(a);
				f.height = f.size + f.lineSpacing;
	
				m = m->NextSiblingElement("font");  ++i;
			}
			vFonts.emplace_back(fs);
			mapFonts[fs.name] = vFonts.size();
		}
		n = n->NextSiblingElement("FontSet");
	}
	Log("FontSets loaded: " + i2s(vFonts.size()));
	
	Log("ThemeSets Loaded from: " + file);
	return true;
}


///  Save
//------------------------------------------------------------------------------------------------
bool Themes::Save() const
{
	using tinyxml2::XMLElement;
	XMLDocument xml;
	XMLElement* root = xml.NewElement("cAmp2");
	XMLElement* e, *p;

	//  time
	int i = 0;
	for (const auto& tc : vTime)
	{
		e = xml.NewElement("TimeColors");
		e->SetAttribute("name", tc.name.c_str());
		if (curTime == i)
			e->SetAttribute("default", 1);
		
		for (const auto& c : tc.vc)
		{
			p = xml.NewElement("clr");
			p->SetAttribute("time", f2s(c.t, 3,5).c_str());
			string rgb = f2s(c.r, 3,5) +" "+f2s(c.g, 3,5) +" "+f2s(c.b, 3,5);
			p->SetAttribute("rgb", rgb.c_str());
			e->InsertEndChild(p);
		}
		root->InsertEndChild(e);  ++i;
	}
	
	//  visual
	i = 0;
	for (const auto& vc : vVisual)
	{
		e = xml.NewElement("VisualColors");
		e->SetAttribute("name", vc.name.c_str());
		if (i == curFFT)  e->SetAttribute("curFFT", 1);
		if (i == curSpect)  e->SetAttribute("curSpect", 1);
		
		auto Save = [&](Float3HSV hsv, int type)
		{
			p = xml.NewElement("hsv");
			p->SetAttribute("type", type);
			p->SetAttribute("h", f2s(hsv.h, 3,5).c_str());
			p->SetAttribute("s", f2s(hsv.s, 3,5).c_str());
			p->SetAttribute("v", f2s(hsv.v, 3,5).c_str());
			e->InsertEndChild(p);
		};
		Save(vc.add, 0);
		Save(vc.mul, 1);
		Save(vc.pow, 2);

		root->InsertEndChild(e);  ++i;
	}

	//  fonts
	for (const auto& fs : vFonts)
	{
		e = xml.NewElement("FontSet");
		e->SetAttribute("name", fs.name.c_str());
		
		for (i=0; i < Fnt_ALL; ++i)
		{
			const auto& f = fs.fnt[i];
			p = xml.NewElement("font");
			p->SetAttribute("name", f.name.c_str());
			p->SetAttribute("size", i2s(f.size).c_str());
			p->SetAttribute("lspc", i2s(f.lineSpacing).c_str());
			p->SetAttribute("bold", b2s(f.bold).c_str());
			e->InsertEndChild(p);
		};
		root->InsertEndChild(e);
	}
	
	
	//  save  .......
	xml.InsertEndChild(root);
	
	string file = FileSystem::ThemesUser();
	bool ok = xml.SaveFile(file.c_str()) == XML_SUCCESS;
	if (ok)
		Log("ThemeSets Saved.");
	else
		Error("Error saving ThemeSets file: " + file);
	return ok;
}
