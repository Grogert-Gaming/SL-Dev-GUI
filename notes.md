server:
	server dir
	plugins install dir
	start
	stop
	restart
	status box
		"Server starting..."
		"Server running..."
		"Server stopping..."
		"Server stopped..."
		"Server restarting..."
		"Error. Check logs."
plugins:
	selectable list
		add new
	vs proj dir
	open in vs
	build solution(?)
		builds selected project
		^ lock current window, make seperate msg box for status?
	update plugin
		copies dll from vs proj to plugins install dir
notes:
	This is made for only EXILED or only NWAPI
	Will need to determine OS in order to properly open, start, and build