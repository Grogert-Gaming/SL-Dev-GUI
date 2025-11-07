#ifndef MANAGER_FRAME_H
#define MANAGER_FRAME_H

#include "macros.h"
#include <wx/wx.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <filesystem>
#include <fstream>

class ManagerFrame : public wxFrame {
public:
	ManagerFrame();

	~ManagerFrame();
private:
	// Dynamic Elements
	wxListBox* lbxPlugins;
	wxTextCtrl* txtServerDir;
	wxTextCtrl* txtPluginsDir;
	wxTextCtrl* txtProjDir;
	wxStaticText* lblServerStatus;
	wxStaticText* lblBuildStatus;

	// Element string values
	wxString selectedPlugin;
	wxString serverDir;
	wxString pluginsDir;
	wxString projDir;

	// Event Handlers
	void OnAddPlugin(wxCommandEvent& ev);
	void OnSetServerDir(wxCommandEvent& ev);
	void OnSetPluginsDir(wxCommandEvent& ev);
	void OnSetProjDir(wxCommandEvent& ev);
	
	void OnStartServer(wxCommandEvent& ev);
	void OnStopServer(wxCommandEvent& ev);
	void OnRestartServer(wxCommandEvent& ev);
	void OnOpenProject(wxCommandEvent& ev);
	void OnBuildDLL(wxCommandEvent& ev);
	void OnUpdateDLL(wxCommandEvent& ev);

	// Modify the state of the GUI elements
	void LoadConfig();
	void SaveConfig();

	// Extras
	wxStandardID GetDir(std::string title, wxString& dir, wxTextCtrl*& ctrl);
};

#endif // MANAGER_FRAME_H