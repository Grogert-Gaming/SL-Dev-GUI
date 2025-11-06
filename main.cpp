#include <wx/wx.h>  
#include <wx/frame.h>

class TestApp : public wxApp {  
public:  
	virtual bool OnInit() {  
		wxFrame * frame = new wxFrame(nullptr, wxID_ANY, "Hello, world!");  
		frame->SetSize(400, 300);  
		frame->Show(true);  
		return true;  
	}  
};  

wxIMPLEMENT_APP(TestApp);