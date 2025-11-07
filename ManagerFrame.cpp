#include "ManagerFrame.h"

ManagerFrame::ManagerFrame() : wxFrame(nullptr, wxID_ANY,
	"SCP:SL Development Pane v" PRJ_VERS, wxDefaultPosition, wxSize(650, 415), 
	wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX) {
	// Initialize panel and box model
	wxPanel* pnlMain = new wxPanel(this);
	wxBoxSizer* szrMain = new wxBoxSizer(wxHORIZONTAL);

	// Left panel (select plugins)
	wxPanel* pnlLeft = new wxPanel(pnlMain);
	wxBoxSizer* szrLeft = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* szrProjDir = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* lblProjDir = new wxStaticText(pnlLeft, wxID_ANY, "Plugin Path:");
	txtProjDir = new wxTextCtrl(pnlLeft, wxID_ANY, "Selected Plugin Path");
	wxButton* btnProjDirBrowse = new wxButton(pnlLeft, wxID_ANY, "...", wxDefaultPosition, wxSize(30, -1));
	btnProjDirBrowse->SetToolTip("Browse");
	szrProjDir->Add(lblProjDir, 0, wxLeft, 5);
	szrProjDir->Add(txtProjDir, 1, wxEXPAND);
	szrProjDir->Add(btnProjDirBrowse, 0, wxLeft, 5);
	
	elbPlugins = new wxEditableListBox(pnlLeft, wxID_ANY, "Plugins");

	// Disable text controls
	txtProjDir->SetEditable(false);

	szrLeft->Add(szrProjDir, 0, wxALL | wxEXPAND, 5);
	szrLeft->Add(elbPlugins, 1, wxALL | wxEXPAND, 5);

	pnlLeft->SetSizer(szrLeft);
	szrMain->Add(pnlLeft, 1, wxEXPAND | wxALL, 5);

	// Right panel (btn controls)
	wxPanel* pnlRight = new wxPanel(pnlMain);
	wxBoxSizer* szrRight = new wxBoxSizer(wxVERTICAL);

	wxButton* btnOpenProj = new wxButton(pnlRight, wxID_ANY, "Open Project");
	wxButton* btnBuildDLL = new wxButton(pnlRight, wxID_ANY, "Build Plugin");
	wxButton* btnUpdateDLL = new wxButton(pnlRight, wxID_ANY, "Update Plugin");

	wxBoxSizer* szrPluginsDir = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* lblPluginsDir = new wxStaticText(pnlRight, wxID_ANY, "Plugins Folder:");
	txtPluginsDir = new wxTextCtrl(pnlRight, wxID_ANY, "Plugins Directory");
	wxButton* btnPluginsDirBrowse = new wxButton(pnlRight, wxID_ANY, "...", wxDefaultPosition, wxSize(30, -1));
	btnPluginsDirBrowse->SetToolTip("Browse");
	szrPluginsDir->Add(lblPluginsDir, 0, wxLeft, 5);
	szrPluginsDir->Add(txtPluginsDir, 1, wxEXPAND);
	szrPluginsDir->Add(btnPluginsDirBrowse, 0, wxLeft, 5);
	
	wxStaticText* lblBuildStatus_0 = new wxStaticText(pnlRight, wxID_ANY, "Build Status:");
	lblBuildStatus = new wxStaticText(pnlRight, wxID_ANY, STATUS_BUILD_NEVER);
	wxStaticText* lblServerStatus_0 = new wxStaticText(pnlRight, wxID_ANY, "Server Status:");
	lblServerStatus = new wxStaticText(pnlRight, wxID_ANY, STATUS_SRV_OFF);

	wxBoxSizer* szrServerDir = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* lblServerDir = new wxStaticText(pnlRight, wxID_ANY, "Server Folder:");
	txtServerDir = new wxTextCtrl(pnlRight, wxID_ANY, "Server Directory");
	wxButton* btnServerDirBrowse = new wxButton(pnlRight, wxID_ANY, "...", wxDefaultPosition, wxSize(30, -1));
	btnServerDirBrowse->SetToolTip("Browse");
	szrServerDir->Add(lblServerDir, 0, wxLeft, 5);
	szrServerDir->Add(txtServerDir, 1, wxEXPAND);
	szrServerDir->Add(btnServerDirBrowse, 0, wxLeft, 5);
	
	wxButton* btnStart = new wxButton(pnlRight, wxID_ANY, "Start");
	wxButton* btnStop = new wxButton(pnlRight, wxID_ANY, "Stop");
	wxButton* btnRestart = new wxButton(pnlRight, wxID_ANY, "Restart");

	// Disable text controls
	txtPluginsDir->SetEditable(false);
	txtServerDir->SetEditable(false);

	szrRight->Add(btnOpenProj, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnBuildDLL, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnUpdateDLL, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(szrPluginsDir, 0, wxALL | wxEXPAND, 5);

	szrRight->Add(lblBuildStatus_0, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(lblBuildStatus, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(lblServerStatus_0, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(lblServerStatus, 0, wxALL | wxEXPAND, 5);

	szrRight->Add(szrServerDir, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnStart, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnStop, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnRestart, 0, wxALL | wxEXPAND, 5);

	pnlRight->SetSizer(szrRight);
	szrMain->Add(pnlRight, 1, wxEXPAND | wxALL, 5);

	pnlMain->SetSizer(szrMain);

	// Bind Event Handlers
	elbPlugins->Bind(wxEVT_LIST_ITEM_SELECTED, &ManagerFrame::OnPluginSelected, this);
	elbPlugins->Bind(wxEVT_LIST_DELETE_ITEM, &ManagerFrame::OnPluginRemoved, this);
	elbPlugins->Bind(wxEVT_LIST_INSERT_ITEM, &ManagerFrame::OnPluginAdded, this);
	elbPlugins->Bind(wxEVT_LIST_END_LABEL_EDIT, &ManagerFrame::OnPluginRenamed, this);

	btnServerDirBrowse->Bind(wxEVT_BUTTON, &ManagerFrame::OnSetServerDir, this);
	btnPluginsDirBrowse->Bind(wxEVT_BUTTON, &ManagerFrame::OnSetPluginsDir, this);
	btnProjDirBrowse->Bind(wxEVT_BUTTON, &ManagerFrame::OnSetProjDir, this);

	btnStart->Bind(wxEVT_BUTTON, &ManagerFrame::OnStartServer, this);
	btnStop->Bind(wxEVT_BUTTON, &ManagerFrame::OnStopServer, this);
	btnRestart->Bind(wxEVT_BUTTON, &ManagerFrame::OnRestartServer, this);
	btnOpenProj->Bind(wxEVT_BUTTON, &ManagerFrame::OnOpenProject, this);
	btnBuildDLL->Bind(wxEVT_BUTTON, &ManagerFrame::OnBuildDLL, this);
	btnUpdateDLL->Bind(wxEVT_BUTTON, &ManagerFrame::OnUpdateDLL, this);

	// Load last config
	//LoadConfig();
}

ManagerFrame::~ManagerFrame() {
	// Save current config
	//SaveConfig();
}

void ManagerFrame::OnPluginAdded(wxListEvent& ev) {
	ev.Skip();

	// Make sure the item was added before mapping
	CallAfter([this]() {

	});
}

void ManagerFrame::OnPluginRemoved(wxListEvent& ev) {
	ev.Skip();

	// Make sure the item was removed before mapping
	CallAfter([this]() {

	});
}

void ManagerFrame::OnPluginSelected(wxListEvent& ev) {
	ev.Skip();

	// Make sure the item was selected before checking the map
	CallAfter([this]() {

	});
}

void ManagerFrame::OnPluginRenamed(wxListEvent& ev) {
	// Can we use the index or do we have to use the old name?
	// ^ to update the map
	ev.Skip();

	// Make sure the item was updated before updating the map
	CallAfter([this]() {

	});
}

void ManagerFrame::OnSetServerDir(wxCommandEvent& ev) {
	GetDir("Select Server Directory", serverDir, txtServerDir);

}

void ManagerFrame::OnSetPluginsDir(wxCommandEvent& ev) {
	GetDir("Select Plugins Directory", pluginsDir, txtPluginsDir);
}

void ManagerFrame::OnSetProjDir(wxCommandEvent& ev) {
	GetDir("Select Project Directory", projDir, txtProjDir);
}

void ManagerFrame::OnStartServer(wxCommandEvent& ev) {

}

void ManagerFrame::OnStopServer(wxCommandEvent& ev) {

}

void ManagerFrame::OnRestartServer(wxCommandEvent& ev) {

}

void ManagerFrame::OnOpenProject(wxCommandEvent& ev) {

}

void ManagerFrame::OnBuildDLL(wxCommandEvent& ev) {

}

void ManagerFrame::OnUpdateDLL(wxCommandEvent& ev) {

}

void ManagerFrame::LoadConfig() {
	std::ifstream fi(SAVE_FILE);
	
	if (fi.is_open()) {
		std::string li;
		if (std::getline(fi, li)) serverDir = wxString(li);
		if (std::getline(fi, li)) pluginsDir = wxString(li);
		if (std::getline(fi, li)) projDir = wxString(li);

		while (std::getline(fi, li))
			if (!li.empty()) lbxPlugins->Append(wxString(li));

		if (serverDir.IsEmpty()) txtServerDir->SetValue(serverDir);
		if (pluginsDir.IsEmpty()) txtPluginsDir->SetValue(pluginsDir);
		if (projDir.IsEmpty()) txtProjDir->SetValue(projDir);
	}
}

void ManagerFrame::SaveConfig() {
	std::ofstream fi(SAVE_FILE);
	
	if (fi.is_open()) {
		fi << serverDir.ToStdString() << "\n";
		fi << pluginsDir.ToStdString() << "\n";
		fi << projDir.ToStdString() << "\n";

		for (size_t i = 0; i < lbxPlugins->GetCount(); i++)
			fi << lbxPlugins->GetString(i).ToStdString() << "\n";
	}
}

wxStandardID ManagerFrame::GetDir(std::string title, wxString& dir, wxTextCtrl*& ctrl) {
	wxDirDialog dialog(this, title);

	wxStandardID resp = (wxStandardID) dialog.ShowModal();

	if (resp == wxID_OK) {
		dir = dialog.GetPath();
		ctrl->SetValue(dir);
		ctrl->SetToolTip(dir);
	}

	return resp;
}

wxString ManagerFrame::GetListItem(size_t index) {
	wxArrayString items;

	elbPlugins->GetStrings(items);

	if (items.IsEmpty() || index > items.GetCount()) return wxEmptyString;
	
	return items[index];
}

wxString ManagerFrame::GetLastListItem() {
	wxArrayString items;

	elbPlugins->GetStrings(items);

	if (items.IsEmpty()) return wxEmptyString;

	return items.Last();
}
