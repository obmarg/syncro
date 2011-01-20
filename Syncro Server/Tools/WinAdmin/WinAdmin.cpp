// WinAdmin.cpp : main project file.

#include "stdafx.h"
#include "FolderListForm.h"

using namespace WinAdmin;

[STAThreadAttribute]
int main( array < System::String ^ > ^ args )
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault( false );

	// Create the main window and run it
	Application::Run( gcnew FolderListForm() );
	return 0;
}
