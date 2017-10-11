#include "util.h"

#include <Windows.h>

#include <DXUT.h>


std::wstring GetExePath()
{
	// get full path to .exe
	const size_t bufferSize = 1024;
	wchar_t buffer[bufferSize];
	if(0 == GetModuleFileNameW(nullptr, buffer, bufferSize))
	{
		return std::wstring(L"");
	}
	std::wstring path(buffer);
	// extract path (remove filename)
	size_t posSlash = path.find_last_of(L"/\\");
	if(posSlash != std::wstring::npos)
	{
		path = path.substr(0, posSlash + 1);
	}
	return path;
}


void UpdateWindowTitle(const std::wstring& appName)
{
	// check if we should update the window title
	bool update = false;

	// update if window size changed
	static int s_windowWidth = 0;
	static int s_windowHeight = 0;
	if (s_windowWidth != DXUTGetWindowWidth() || s_windowHeight != DXUTGetWindowHeight()) {
		s_windowWidth = DXUTGetWindowWidth();
		s_windowHeight = DXUTGetWindowHeight();
		update = true;
	}

	// update if fps changed (updated once per second by DXUT)
	static float s_fps = 0.0f;
	static float s_mspf = 0.0f;
	if (s_fps != DXUTGetFPS()) {
		s_fps = DXUTGetFPS();
		s_mspf = 1000.0f / s_fps;
		update = true;
	}

	// update window title if something relevant changed
	if (update) {
		const size_t len = 512;
		wchar_t str[len];
		swprintf_s(str, len, L"%s %ux%u @ %.2f fps / %.2f ms", appName.c_str(), s_windowWidth, s_windowHeight, s_fps, s_mspf);
		SetWindowText(DXUTGetHWND(), str);
	}
}
