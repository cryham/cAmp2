#include "AppSFML_Draw.h"
using namespace std;


int main()
{
	unique_ptr<AppSFMLDraw> app = make_unique<AppSFMLDraw>();
	bool ok = app->Run();
	
	return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
