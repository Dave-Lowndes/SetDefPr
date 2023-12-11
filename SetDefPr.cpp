#include <windows.h>
#include <tchar.h>

constexpr TCHAR szAppName[] = _T("Set Default Printer Utility");

int CALLBACK WinMain( _In_ HINSTANCE /*hInst*/, _In_opt_ HINSTANCE /*hPrevInst*/, _In_ LPSTR /*lpCmdLine*/, _In_ int /*nShowCmd*/ )
{
	/* Parameter check */
	if ( __argc != 2 )
	{
		MessageBox( NULL, _T("Usage:\n\nSetDefPr \"Printer Device Name\"\n\nNote: The printer device name must match the case of the name precisely."),
			szAppName, MB_OK | MB_ICONINFORMATION );
		return 0;
	}

	/* See if we can find the specified printer in order to get the port & device driver string */
	TCHAR szDeviceString[260];

	/* First (and only) parameter is the printer name */
	lstrcpyn( szDeviceString, __argv[1], _countof(szDeviceString)-1 );
	const int Pos = lstrlen( szDeviceString );

	/* Append a ',' */
	szDeviceString[ Pos ] = ',';

	GetProfileString( "Devices", __argv[1], "", &szDeviceString[Pos+1], _countof( szDeviceString ) - (Pos+1) );

	/* Have we got the printer? */
	if ( szDeviceString[Pos+1] != '\0' )
	{
		// To set the default printer for Windows 95 use:
		if ( WriteProfileString( "windows", "device", szDeviceString ) )
		{
			SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE/*WM_WININICHANGE*/,
						0L, (LPARAM)(LPCTSTR)"windows", SMTO_NORMAL, 1000, NULL );
		}
		else
		{
			MessageBox( NULL, _T("Failed to update device setting"), szAppName, MB_OK | MB_ICONERROR );
		}
	}
	else
	{
		MessageBox( NULL, "Printer not found", szAppName, MB_OK | MB_ICONERROR );
	}

	// To set the default printer for Windows NT use:
	/* Note printer driver is usually WINSPOOL under Windows NT */
//	WriteProfileString("windows", "device", "My Printer,WINSPOOL,lpt1:");
//	SendMessageTimeout(HWND_BROADCAST, WM_WININICHANGE, 0L, 0L,
//	SMTO_NORMAL, 1000, NULL);

	return 0;
}