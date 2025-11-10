#ifndef MANAGER_FRAME_H
#define MANAGER_FRAME_H

#include "macros.h"
#include <wx/wx.h>
#include <wx/listbox.h>
#include <wx/editlbox.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <unordered_map>
#include <filesystem>
#include <fstream>

class ManagerFrame : public wxFrame {
public:
	ManagerFrame();

	~ManagerFrame();
private:
	// Dynamic Elements
	wxEditableListBox* elbPlugins;
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

	// Plugin map
	std::unordered_map<wxString, wxString> plugins;

	// Event Handlers
	void OnPluginAdded(wxListEvent& ev);
	void OnPluginRemoved(wxListEvent& ev);
	void OnPluginSelected(wxListEvent& ev);
	void OnPluginRenamed(wxListEvent& ev);

	void OnSetServerDir(wxCommandEvent& ev);
	void OnSetPluginsDir(wxCommandEvent& ev);
	void OnSetProjDir(wxCommandEvent& ev);

	void OnOpenServerDir(wxCommandEvent& ev);
	void OnOpenPluginsDir(wxCommandEvent& ev);
	void OnOpenProjDir(wxCommandEvent& ev);
	
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
	wxString GetListItem(size_t index);
	wxString GetLastListItem();
};

#endif // MANAGER_FRAME_H