#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <wx/wx.h>
#include <wx/process.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/datetime.h>

class SystemManager {
public:
	SystemManager();
	~SystemManager();

	bool Start(const wxString& serverExePath);
	bool Stop();
	bool Restart();
	bool IsSrvRunning() const;
	long GetSrvPID() const;

	bool OpenProject(const wxString& projDir); // use this to make a progress bar
	bool BuildDLL(const wxString& projDir, wxString& buildTime, wxString& errMsg); // use this to make a progress bar
	bool UpdateDLL(const wxString& projDir, const wxString& pluginsDir, wxString name, wxString&errMsg); // use this to make a progress bar

private:
	wxProcess* srvProcess;
	long srvPID;
	wxString lastServerExePath;

	wxString FindSLN(const wxString& projDir);
	wxString FindDLL(const wxString& projDir, wxString name);
};

#endif // SYSTEM_MANAGER_H