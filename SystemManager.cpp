#include "SystemManager.h"

SystemManager::SystemManager() : srvProcess(nullptr), srvPID(0) {}

SystemManager::~SystemManager() {
	if (srvProcess) Stop();
}

bool SystemManager::Start(const wxString& serverExePath) {
	if (srvProcess) return false;
	if (serverExePath.IsEmpty() || !wxFileExists(serverExePath)) return false;

	// Stored for restarts
	lastServerExePath = serverExePath;

	wxString cmd;

#ifdef _WIN32 // Windows systems
	if (serverExePath.EndsWith(".bat") || serverExePath.EndsWith(".cmd"))
		cmd = "\"" + serverExePath + "\"";
	else
		cmd = "\"" + serverExePath + "\"";
	srvPID = wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_SHOW_CONSOLE | wxEXEC_MAKE_GROUP_LEADER, srvProcess);
#else // Linux/Mac systems
	// Use x-terminal-emulator (Debian/Ubuntu standard) or fallback to xterm
	cmd = "x-terminal-emulator -e \"" + serverExePath + "\"";
	srvPID = wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_MAKE_GROUP_LEADER, srvProcess);
#endif

	// Start the server
	srvProcess = new wxProcess();

	// Start the server
	srvProcess = new wxProcess();

	srvPID = wxExecute(cmd, wxEXEC_ASYNC, srvProcess);

	if (srvPID == 0) {
		delete srvProcess;
		srvProcess = nullptr;
		return false;
	}

	return true;
}

bool SystemManager::Stop() {
	if (!srvProcess) return false;

	// Send SIGTERM rather than SIGKILL to protect the server
	wxKillError killErr = wxProcess::Kill(srvPID, wxSIGTERM, wxKILL_CHILDREN);
	// Allow for cleanup
	wxMilliSleep(1000);

	// Process didn't die
	if (killErr != wxKILL_OK && killErr != wxKILL_NO_PROCESS) return false;

	delete srvProcess;
	srvProcess = nullptr;
	srvPID = 0;

	return true;
}

bool SystemManager::Restart() {
	if (!srvProcess) return false;

	if (!Stop()) return false;

	return Start(lastServerExePath);
}

bool SystemManager::IsSrvRunning() const {
	return srvProcess != nullptr;
}

long SystemManager::GetSrvPID() const {
	return srvPID;
}

bool SystemManager::OpenProject(const wxString& projDir) {
	if (projDir.IsEmpty() || !wxDirExists(projDir)) return false;

	wxString sln = FindSLN(projDir);
	if (sln.IsEmpty()) return false;

	return wxLaunchDefaultApplication(sln);
}

bool SystemManager::BuildDLL(const wxString& projDir, wxString& buildTime, wxString& errMsg) {
	if (projDir.IsEmpty() || !wxDirExists(projDir)) {
		errMsg = "Project directory does not exist!";
		return false;
	}

	wxString sln = FindSLN(projDir);

	if (sln.IsEmpty()) {
		errMsg = "No solution (.sln) or project (.csproj) file found in \"" + projDir + "\".";
		return false;
	}

	wxString cmd = "dotnet build \"" + sln + "\" -c Debug";

	// Execute build command
	wxArrayString output;
	wxArrayString errors;

	long exitCode = wxExecute(cmd, output, errors, wxEXEC_SYNC);

	if (exitCode != 0) {
		errMsg = "Build failed with exit code: " + exitCode;

		if (!errors.IsEmpty()) {
			errMsg += "\n\nErrors:\n";
			for (const auto& error : errors)
				errMsg += error + "\n";
		}

		return false;
	}

	wxDateTime now = wxDateTime::Now();
	buildTime = now.FormatISOCombined(' ');
	return true;
}

bool SystemManager::UpdateDLL(const wxString& projDir, const wxString& pluginsDir, wxString name, wxString& errMsg) {
	if (projDir.IsEmpty() || !wxDirExists(projDir)) {
		errMsg = "Project [source] directory does not exist!";
		return false;
	}

	if (pluginsDir.IsEmpty() || !wxDirExists(pluginsDir)) {
		errMsg = "Plugins [target] directory does not exist!";
		return false;
	}

	wxString dllPath = FindDLL(projDir, name);

	if (dllPath.IsEmpty()) {
		errMsg = "No DLL found in \"" + projDir + "\"!\nPlease build the project before updating the DLL";
		return false;
	}

	wxString dllName = wxFileName(dllPath).GetFullName();
	wxString destPath = wxFileName(pluginsDir, dllName).GetFullPath();
	//wxMessageBox("dllName: " + dllName + " | destPath: " + destPath + " | dllPath: " + dllPath);
	//wxMessageBox(dllName);
	//wxMessageBox(destPath);
	//wxMessageBox(dllPath);
	if (!wxCopyFile(dllPath, destPath, true)) {
		errMsg = "Failed to copy DLL from \"" + dllPath +"\" to \"" + destPath + "\"!";
		return false;
	}

	return true;
}

wxString SystemManager::FindSLN(const wxString& projDir) {
	wxDir dir(projDir);

	if (!dir.IsOpened()) return wxEmptyString;

	wxString file;
	bool found = false;

	// Look for .sln
	found = dir.GetFirst(&file, "*.sln", wxDIR_FILES);
	if (found) return wxFileName(projDir, file).GetFullPath();

	// Look for .csproj
	found = dir.GetFirst(&file, "*.csproj", wxDIR_FILES);
	if (found) return wxFileName(projDir, file).GetFullPath();

	return wxEmptyString;
}

wxString SystemManager::FindDLL(const wxString& projDir, wxString pluginName) {
	wxString projName = wxFileName(projDir + "\\").GetDirs().Last();
	wxArrayString paths = {
		projDir + "\\bin\\Debug",
		projDir + "\\" + projName + "\\bin\\Debug"
		//wxFileName(projDir, "/bin/Debug/test").GetFullPath(),
		//wxFileName(projDir, "bin/Release").GetFullPath(),
		//wxFileName(projDir, projDir+"/bin/Debug").GetFullPath(),
		//wxFileName(projDir, projDir+"/bin/Release").GetFullPath(),
	};

	wxMessageBox(projName);
	wxMessageBox(paths[0] + "\n" + paths[1]);

	for (const wxString& search : paths) {
		if (!wxDirExists(search)) continue;

		wxDir dir(search);
		if (!dir.IsOpened()) continue;

		wxString file;
		bool found = dir.GetFirst(&file, pluginName + ".dll", wxDIR_FILES);

		if (found) {
			return wxFileName(search, file).GetFullPath();
		}
	}

	return wxEmptyString;
}
