#include "ManagerFrame.h"

class ManagerApp : public wxApp {
public:
	virtual bool OnInit() {
		ManagerFrame* frame = new ManagerFrame();
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(ManagerApp);
