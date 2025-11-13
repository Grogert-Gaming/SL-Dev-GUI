#include "ManagerFrame.h"

ManagerFrame::ManagerFrame() : wxFrame(nullptr, wxID_ANY,
	"SCP:SL Development Pane v" PRJ_VERS, wxDefaultPosition, wxSize(650, 485), 
	wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX) {
	// System initialization
	sysMan = new SystemManager();

	// Initialize panel and box model
	wxPanel* pnlMain = new wxPanel(this);
	wxBoxSizer* szrMain = new wxBoxSizer(wxHORIZONTAL);

	// Left panel (select plugins)
	wxPanel* pnlLeft = new wxPanel(pnlMain);
	wxBoxSizer* szrLeft = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* szrProjDir = MakeBrowseControl(pnlLeft, txtProjDir, &ManagerFrame::OnSetProjDir, "Plugin Folder:", TXT_PROJ_PLACEHOLDER);
	wxButton* btnProjDirOpen = new wxButton(pnlLeft, wxID_ANY, "Project Folder");
	
	elbPlugins = new wxEditableListBox(pnlLeft, wxID_ANY, "Plugins");

	szrLeft->Add(szrProjDir, 0, wxALL | wxEXPAND, 5);
	szrLeft->Add(btnProjDirOpen, 0, wxALL | wxEXPAND, 5);
	szrLeft->Add(elbPlugins, 1, wxALL | wxEXPAND, 5);

	pnlLeft->SetSizer(szrLeft);
	szrMain->Add(pnlLeft, 1, wxEXPAND | wxALL, 5);

	// Right panel (btn controls)
	wxPanel* pnlRight = new wxPanel(pnlMain);
	wxBoxSizer* szrRight = new wxBoxSizer(wxVERTICAL);

	wxButton* btnOpenProj = new wxButton(pnlRight, wxID_ANY, "Open Project");
	wxButton* btnBuildDLL = new wxButton(pnlRight, wxID_ANY, "Build Plugin");
	wxButton* btnUpdateDLL = new wxButton(pnlRight, wxID_ANY, "Update Plugin");
	wxButton* btnPluginsDirOpen = new wxButton(pnlRight, wxID_ANY, "Open Plugins Folder");

	wxBoxSizer* szrPluginsDir = MakeBrowseControl(pnlRight, txtPluginsDir, &ManagerFrame::OnSetPluginsDir, "Plugins Folder:", TXT_PLUGINS_PLACEHOLDER);
	
	wxStaticText* lblBuildStatus_0 = new wxStaticText(pnlRight, wxID_ANY, "Build Status:");
	lblBuildStatus = new wxStaticText(pnlRight, wxID_ANY, STATUS_BUILD_NEVER);
	wxStaticText* lblServerStatus_0 = new wxStaticText(pnlRight, wxID_ANY, "Server Status:");
	lblServerStatus = new wxStaticText(pnlRight, wxID_ANY, STATUS_SRV_OFF);

	wxBoxSizer* szrServerDir = MakeBrowseControl(pnlRight, txtServerDir, &ManagerFrame::OnSetServerDir, "Server Folder:", TXT_SRV_PLACEHOLDER);
	wxBoxSizer* szrServerExePath = MakeBrowseControl(pnlRight, txtServerExePath, &ManagerFrame::OnSetServerExePath, "Server Script:", TXT_EXE_PLACEHOLDER);

	wxButton* btnServerDirOpen = new wxButton(pnlRight, wxID_ANY, "Open Server Folder");
	
	wxBoxSizer* szrServerControls = new wxBoxSizer(wxHORIZONTAL);
	wxButton* btnStart = new wxButton(pnlRight, wxID_ANY, "Start");
	wxButton* btnStop = new wxButton(pnlRight, wxID_ANY, "Stop");
	szrServerControls->Add(btnStart, 1, wxEXPAND);
	szrServerControls->Add(btnStop, 1, wxEXPAND);

	wxButton* btnRestart = new wxButton(pnlRight, wxID_ANY, "Restart");

	szrRight->Add(btnOpenProj, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnBuildDLL, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnUpdateDLL, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnPluginsDirOpen, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(szrPluginsDir, 0, wxALL | wxEXPAND, 5);

	szrRight->Add(lblBuildStatus_0, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(lblBuildStatus, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(lblServerStatus_0, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(lblServerStatus, 0, wxALL | wxEXPAND, 5);

	szrRight->Add(szrServerDir, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(szrServerExePath, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnServerDirOpen, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(szrServerControls, 0, wxALL | wxEXPAND, 5);
	szrRight->Add(btnRestart, 0, wxALL | wxEXPAND, 5);

	pnlRight->SetSizer(szrRight);
	szrMain->Add(pnlRight, 1, wxEXPAND | wxALL, 5);

	pnlMain->SetSizer(szrMain);

	// Bind Event Handlers
	elbPlugins->Bind(wxEVT_LIST_ITEM_SELECTED, &ManagerFrame::OnPluginSelected, this);
	elbPlugins->Bind(wxEVT_LIST_DELETE_ITEM, &ManagerFrame::OnPluginRemoved, this);
	elbPlugins->Bind(wxEVT_LIST_INSERT_ITEM, &ManagerFrame::OnPluginAdded, this);
	elbPlugins->Bind(wxEVT_LIST_END_LABEL_EDIT, &ManagerFrame::OnPluginRenamed, this);

	btnServerDirOpen->Bind(wxEVT_BUTTON, &ManagerFrame::OnOpenServerDir, this);
	btnPluginsDirOpen->Bind(wxEVT_BUTTON, &ManagerFrame::OnOpenPluginsDir, this);
	btnProjDirOpen->Bind(wxEVT_BUTTON, &ManagerFrame::OnOpenProjDir, this);

	btnStart->Bind(wxEVT_BUTTON, &ManagerFrame::OnStartServer, this);
	btnStop->Bind(wxEVT_BUTTON, &ManagerFrame::OnStopServer, this);
	btnRestart->Bind(wxEVT_BUTTON, &ManagerFrame::OnRestartServer, this);
	btnOpenProj->Bind(wxEVT_BUTTON, &ManagerFrame::OnOpenProject, this);
	btnBuildDLL->Bind(wxEVT_BUTTON, &ManagerFrame::OnBuildDLL, this);
	btnUpdateDLL->Bind(wxEVT_BUTTON, &ManagerFrame::OnUpdateDLL, this);

	// Load last config
	LoadConfig();
}

ManagerFrame::~ManagerFrame() {
	// Save the current config
	SaveConfig();

	// Cleanup memory
	delete sysMan;
}

void ManagerFrame::OnPluginAdded(wxListEvent& ev) {
	// All new events handled in plugin renamed due to missing text in the event item
	ev.Skip();
}

void ManagerFrame::OnPluginRemoved(wxListEvent& ev) {
	ev.Skip();

	// Make sure the item was removed before mapping
	CallAfter([this]() {
		plugins.erase(selectedPlugin);
	});
}

void ManagerFrame::OnPluginSelected(wxListEvent& ev) {
	ev.Skip();

	// Make sure the item was selected before checking the map
	CallAfter([this, ev]() {
		selectedPlugin = ev.GetItem().GetText();

		// User is adding a new plugin
		if (selectedPlugin == wxEmptyString) {
			txtProjDir->SetValue(TXT_PROJ_PLACEHOLDER);
			return;
		}

		projDir = plugins[selectedPlugin];
		// Ensure that the projDir does not get set as an empty string
		projDir = projDir == wxEmptyString? TXT_PROJ_PLACEHOLDER : projDir;

		txtProjDir->SetValue(projDir);
		txtProjDir->SetToolTip(projDir == TXT_PROJ_PLACEHOLDER? "" : projDir);
	});
}

void ManagerFrame::OnPluginRenamed(wxListEvent& ev) {
	wxString name = ev.GetItem().GetText();
	name.Trim();

	// Selected plugin should call first, updating the selected plugin
	// If the user is selecting an empty string, they are likely adding 
	if (name == wxEmptyString) {
		wxMessageBox("OnPluginRenamed(): Cannot enter an empty string (" + name + ")");
		ev.Veto();
		return;
	}

	// No change was made
	if (name == selectedPlugin) {
		ev.Veto();
		return;
	}

	// The user renamed to an existing name
	if (plugins.find(name) != plugins.end()) {
		wxMessageBox("OnPluginRenamed(): Cannot have two items with the same name (" + name + ")");
		ev.Veto();
		return;
	}

	// Item is currently being added to the list
	if (selectedPlugin == wxEmptyString) {
		plugins[name];
	} else {
		plugins.erase(selectedPlugin);
		plugins[name] = projDir;
	}

	selectedPlugin = name;

	ev.Skip();
	// !!!! Ignore beneath, it is probably wrong !!!!
	// Event name is wxEVT_LIST_END_LABEL_EDIT
	// Does not fire wxEVT_LIST_ITEM_SELECTED before or after a rename event
	//		^ Unless the user is adding a new item, in which case it fires before
	// Fires after wxEVT_LIST_INSERT_ITEM
}

void ManagerFrame::OnSetServerDir(wxCommandEvent& ev) {
	GetDir("Select Server Directory", serverDir, txtServerDir);
}

void ManagerFrame::OnSetPluginsDir(wxCommandEvent& ev) {
	GetDir("Select Plugins Directory", pluginsDir, txtPluginsDir);
}

void ManagerFrame::OnSetProjDir(wxCommandEvent& ev) {
	if (selectedPlugin == wxEmptyString) {
		wxMessageBox("Must select a plugin before adding a project path!");
		return;
	}

	GetDir("Select Project Directory", projDir, txtProjDir);
	plugins[selectedPlugin] = projDir;
}

void ManagerFrame::OnSetServerExePath(wxCommandEvent& ev) {
	GetFile("Select Server Script", serverExePath, txtServerExePath);
}

void ManagerFrame::OnOpenServerDir(wxCommandEvent& ev) {
	if (serverDir.IsEmpty() || serverDir == TXT_SRV_PLACEHOLDER) {
		wxMessageBox("Server directory is not set!");
		return;
	}

	if (!wxDirExists(serverDir)) {
		wxMessageBox("Server directory does not exist!");
		return;
	}

	if (!wxLaunchDefaultApplication(serverDir)) {
		wxMessageBox("Failed to open server directory!");
	}
}

void ManagerFrame::OnOpenPluginsDir(wxCommandEvent& ev) {
	if (pluginsDir.IsEmpty() || pluginsDir == TXT_PLUGINS_PLACEHOLDER) {
		wxMessageBox("Plugins directory is not set!");
		return;
	}

	if (!wxDirExists(pluginsDir)) {
		wxMessageBox("Plugins directory does not exist!");
		return;
	}

	if (!wxLaunchDefaultApplication(pluginsDir)) {
		wxMessageBox("Failed to open plugins directory!");
	}
}

void ManagerFrame::OnOpenProjDir(wxCommandEvent& ev) {
	if (selectedPlugin.IsEmpty()) {
		wxMessageBox("No plugin selected!");
		return;
	}

	if (projDir.IsEmpty() || projDir == TXT_PROJ_PLACEHOLDER) {
		wxMessageBox("Selected plugin does not have a project directory set!");
		return;
	}

	if (!wxDirExists(projDir)) {
		wxMessageBox("Selected project directory does not exist!");
		return;
	}

	if (!wxLaunchDefaultApplication(projDir)) {
		wxMessageBox("Failed to open project directory!");
	}
}

void ManagerFrame::OnStartServer(wxCommandEvent& ev) {
	wxMessageBox("Not yet implemented");
	return;

	if (sysMan->IsSrvRunning()) {
		wxMessageBox("Server is already running");
		return;
	}

	if (serverExePath.IsEmpty() || serverExePath == TXT_EXE_PLACEHOLDER) {
		wxMessageBox("Server script is not yet set");
		return;
	}

	if (!wxFileExists(serverExePath)) {
		wxMessageBox("Server script does not exist");
		return;
	}

	lblServerStatus->SetLabel(STATUS_SRV_START);

	if (!sysMan->Start(serverExePath)) {
		lblServerStatus->SetLabel(STATUS_ERR);
		wxMessageBox("Failed to start server");
	} else {
		lblServerStatus->SetLabel(STATUS_SRV_RUN);
	}
}

void ManagerFrame::OnStopServer(wxCommandEvent& ev) {
	wxMessageBox("Not yet implemented");
	return;

	if (!sysMan->IsSrvRunning()) {
		wxMessageBox("Server is not running");
		return;
	}

	lblServerStatus->SetLabel(STATUS_SRV_STOP);

	if (!sysMan->Stop()) {
		lblServerStatus->SetLabel(STATUS_ERR);
		wxMessageBox("Failed to stop server");
	} else {
		lblServerStatus->SetLabel(STATUS_SRV_OFF);
	}
}

void ManagerFrame::OnRestartServer(wxCommandEvent& ev) {
	wxMessageBox("Not yet implemented");
	return;

	if (!sysMan->IsSrvRunning()) {
		wxMessageBox("Server is not running");
		return;
	}

	lblServerStatus->SetLabel(STATUS_SRV_RESTART);

	if (!sysMan->Restart()) {
		lblServerStatus->SetLabel(STATUS_ERR);
		wxMessageBox("Failed to restart server");
	} else {
		lblServerStatus->SetLabel(STATUS_SRV_RUN);
	}
}

void ManagerFrame::OnOpenProject(wxCommandEvent& ev) {
	if (selectedPlugin.IsEmpty()) {
		wxMessageBox("No plugin selected");
		return;
	}

	if (projDir.IsEmpty() || projDir == TXT_PROJ_PLACEHOLDER) {
		wxMessageBox("Selected plugin does not have a project folder set");
		return;
	}

	if (!wxDirExists(projDir)) {
		wxMessageBox("Selected plugin folder does not exist");
		return;
	}

	if (!sysMan->OpenProject(projDir)) {
		wxMessageBox("No solution (.sln) or project (.csproj) file found in \"" + projDir + "\".");
	}
}

void ManagerFrame::OnBuildDLL(wxCommandEvent& ev) {
	if (selectedPlugin.IsEmpty()) {
		wxMessageBox("No plugin selected");
		return;
	}

	if (projDir.IsEmpty() || projDir == TXT_PROJ_PLACEHOLDER) {
		wxMessageBox("Selected plugin does not have a project folder set");
		return;
	}

	if (!wxDirExists(projDir)) {
		wxMessageBox("Selected plugin folder does not exist");
		return;
	}

	wxMessageBox("Building plugin");

	wxString buildTime, errMsg;

	if (sysMan->BuildDLL(projDir, buildTime, errMsg)) {
		lblBuildStatus->SetLabel(STATUS_BUILD_LAST + buildTime);
		wxMessageBox("Build completed");
	} else {
		lblBuildStatus->SetLabel(STATUS_ERR);
		wxMessageBox(errMsg);
	}
}

void ManagerFrame::OnUpdateDLL(wxCommandEvent& ev) {
	if (selectedPlugin.IsEmpty()) {
		wxMessageBox("No plugin selected");
		return;
	}

	if (projDir.IsEmpty() || projDir == TXT_PROJ_PLACEHOLDER) {
		wxMessageBox("Selected plugin does not have a project folder set");
		return;
	}

	if (pluginsDir.IsEmpty() || pluginsDir == TXT_PLUGINS_PLACEHOLDER) {
		wxMessageBox("Plugins folder is not set");
		return;
	}

	wxString errMsg;

	if (sysMan->UpdateDLL(projDir, pluginsDir, selectedPlugin, errMsg)) {
		wxMessageBox("Plugin updated");
	} else {
		wxMessageBox(errMsg);
	}
}

void ManagerFrame::LoadConfig() {
	std::ifstream fi(SAVE_FILE);
	
	if (!fi.is_open()) return;

	std::string li;

	// Server Directory
	std::getline(fi, li);
	serverDir = li;
	txtServerDir->SetValue(li.empty()? TXT_SRV_PLACEHOLDER : li);
	txtServerDir->SetToolTip(serverDir);

	// Server Script Path
	std::getline(fi, li);
	serverExePath = li;
	txtServerExePath->SetValue(li.empty() ? TXT_EXE_PLACEHOLDER : li);
	txtServerExePath->SetToolTip(serverExePath);

	// Plugins Directory
	std::getline(fi, li);
	pluginsDir = li;
	txtPluginsDir->SetValue(li.empty()? TXT_SRV_PLACEHOLDER : li);
	txtPluginsDir->SetToolTip(pluginsDir);

	wxArrayString pluginNames;

	// Load plugins map (name|dir)
	while (std::getline(fi, li)) {
		if (li.empty()) continue;

		size_t delimPos = li.find('|');
		
		if (delimPos == std::string::npos) continue;

		wxString name = wxString(li.substr(0, delimPos));
		wxString dir = wxString(li.substr(delimPos + 1));

		plugins[name] = dir;
		pluginNames.Add(name);
	}

	if (!pluginNames.IsEmpty()) 
		elbPlugins->SetStrings(pluginNames);

	fi.close();
}

void ManagerFrame::SaveConfig() {
	std::ofstream fo(SAVE_FILE);
	
	fo << serverDir.ToStdString() << "\n";
	fo << serverExePath.ToStdString() << "\n";
	fo << pluginsDir.ToStdString() << "\n";

	for (const auto& plugin : plugins) {
		// delimeter may not work cross-platform
		fo << plugin.first.ToStdString() << "|" << plugin.second.ToStdString() << "\n";
	}

	fo.close();
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

wxStandardID ManagerFrame::GetFile(std::string title, wxString& path, wxTextCtrl*& ctrl) {
	wxFileDialog dialog(this, title);

	wxStandardID resp = (wxStandardID)dialog.ShowModal();

	if (resp == wxID_OK) {
		path = dialog.GetPath();
		ctrl->SetValue(path);
		ctrl->SetToolTip(path);
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

wxBoxSizer* ManagerFrame::MakeBrowseControl(wxPanel*& panel, wxTextCtrl*& ctrlText, 
		void (ManagerFrame::* method)(wxCommandEvent&), 
		std::string label, std::string placeholder) {
	wxBoxSizer* ctrlSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* ctrlLabel = new wxStaticText(panel, wxID_ANY, label);
	ctrlText = new wxTextCtrl(panel, wxID_ANY, placeholder);
	wxButton* ctrlBrowseBtn = new wxButton(panel, wxID_ANY, "...", wxDefaultPosition, wxSize(30, -1));
	
	ctrlText->SetEditable(false);

	ctrlBrowseBtn->SetToolTip("Browse");
	ctrlSizer->Add(ctrlLabel, 0, wxLeft, 5);
	ctrlSizer->Add(ctrlText, 1, wxEXPAND);
	ctrlSizer->Add(ctrlBrowseBtn, 0, wxLeft, 5);

	ctrlBrowseBtn->Bind(wxEVT_BUTTON, method, this);

	return ctrlSizer;
}
