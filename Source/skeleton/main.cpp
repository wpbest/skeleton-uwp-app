//#include "SDL_main.h"
//#include <wrl.h>
//
///* At least one file in any SDL/WinRT app appears to require compilation
//   with C++/CX, otherwise a Windows Metadata file won't get created, and
//   an APPX0702 build error can appear shortly after linking.
//
//   The following set of preprocessor code forces this file to be compiled
//   as C++/CX, which appears to cause Visual C++ 2012's build tools to
//   create this .winmd file, and will help allow builds of SDL/WinRT apps
//   to proceed without error.
//
//   If other files in an app's project enable C++/CX compilation, then it might
//   be possible for SDL_winrt_main_NonXAML.cpp to be compiled without /ZW,
//   for Visual C++'s build tools to create a winmd file, and for the app to
//   build without APPX0702 errors.  In this case, if
//   SDL_WINRT_METADATA_FILE_AVAILABLE is defined as a C/C++ macro, then
//   the #error (to force C++/CX compilation) will be disabled.
//
//   Please note that /ZW can be specified on a file-by-file basis.  To do this,
//   right click on the file in Visual C++, click Properties, then change the
//   setting through the dialog that comes up.
//*/
//#ifndef SDL_WINRT_METADATA_FILE_AVAILABLE
//#ifndef __cplusplus_winrt
//#error SDL_winrt_main_NonXAML.cpp must be compiled with /ZW, otherwise build errors due to missing .winmd files can occur.
//#endif
//#endif
//
///* Prevent MSVC++ from warning about threading models when defining our
//   custom WinMain.  The threading model will instead be set via a direct
//   call to Windows::Foundation::Initialize (rather than via an attributed
//   function).
//
//   To note, this warning (C4447) does not seem to come up unless this file
//   is compiled with C++/CX enabled (via the /ZW compiler flag).
//*/
//#ifdef _MSC_VER
//#pragma warning(disable:4447)
//#endif
//
///* Make sure the function to initialize the Windows Runtime gets linked in. */
//#ifdef _MSC_VER
//#pragma comment(lib, "runtimeobject.lib")
//#endif
//
//int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//{
//    if (FAILED(Windows::Foundation::Initialize(RO_INIT_MULTITHREADED))) {
//        return 1;
//    }
//
//    SDL_WinRTRunApp(SDL_main, NULL);
//    return 0;
//}
#include "pch.h"
#include <wrl.h>
#include <Windows.ApplicationModel.core.h>

using namespace ABI::Windows::ApplicationModel;
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

/* Make sure the function to initialize the Windows Runtime gets linked in. */
//#ifdef _MSC_VER
//#pragma comment(lib, "runtimeobject.lib")
//#endif

/* Prevent MSVC++ from warning about threading models when defining our
   custom WinMain.  The threading model will instead be set via a direct
   call to Windows::Foundation::Initialize (rather than via an attributed
   function).

   To note, this warning (C4447) does not seem to come up unless this file
   is compiled with C++/CX enabled (via the /ZW compiler flag).
*/
#ifdef _MSC_VER
#pragma warning(disable:4447)
#endif

#define qHString(x) Wrappers::HString::MakeReference(x).Get()
#define CoreApplicationClass RuntimeClass_Windows_ApplicationModel_Core_CoreApplication
typedef ITypedEventHandler<Core::CoreApplicationView *, Activation::IActivatedEventArgs *> ActivatedHandler;

extern "C" {
	//int __getmainargs(int *argc, char ***argv, char ***env, int expandWildcards, _startupinfo *info);
	int main(int, char **);
}

int main(int argc, char **argv)
{

}

static int g_mainExitCode;

//static QtMessageHandler defaultMessageHandler;
//static void devMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
//{
//#ifndef Q_OS_WINPHONE
//	static HANDLE shmem = 0;
//	static HANDLE event = 0;
//	if (!shmem)
//		shmem = CreateFileMappingFromApp(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 4096, L"qdebug-shmem");
//	if (!event)
//		event = CreateEventEx(NULL, L"qdebug-event", 0, EVENT_ALL_ACCESS);
//
//	void *data = MapViewOfFileFromApp(shmem, FILE_MAP_WRITE, 0, 4096);
//	memset(data, quint32(type), sizeof(quint32));
//	memcpy_s(static_cast<quint32 *>(data) + 1, 4096 - sizeof(quint32),
//		message.data(), (message.length() + 1) * sizeof(wchar_t));
//	UnmapViewOfFile(data);
//	SetEvent(event);
//#endif // !Q_OS_WINPHONE
//	defaultMessageHandler(type, context, message);
//}

class AppContainer : public Microsoft::WRL::RuntimeClass<Core::IFrameworkView>
{
public:
	AppContainer(int argc, char *argv[]) : m_argc(argc), m_deleteArgv0(false)
	{
		//m_argv.reserve(argc);
		//for (int i = 0; i < argc; ++i) {
		//	// Workaround for empty argv[0] which occurs when WMAppManifest's ImageParams is used
		//	// The second argument is taken to be the executable
		//	if (i == 0 && argc >= 2 && !qstrlen(argv[0])) {
		//		const QByteArray argv0 = QDir::current()
		//			.absoluteFilePath(QString::fromLatin1(argv[1])).toUtf8();
		//		m_argv.append(qstrdup(argv0.constData()));
		//		m_argc -= 1;
		//		m_deleteArgv0 = true;
		//		++i;
		//		continue;
		//	}
		//	m_argv.append(argv[i]);
		//}
	}

	~AppContainer()
	{
//		if (m_deleteArgv0)
//			delete[] m_argv[0];
//		for (int i = m_argc; i < m_argv.size(); ++i)
//			delete[] m_argv[i];
	}

	// IFrameworkView Methods
	HRESULT __stdcall Initialize(Core::ICoreApplicationView *view)
	{
		view->add_Activated(Callback<ActivatedHandler>(this, &AppContainer::onActivated).Get(), &m_activationToken);
		return S_OK;
	}
	HRESULT __stdcall SetWindow(ABI::Windows::UI::Core::ICoreWindow *) { return S_OK; }
	HRESULT __stdcall Load(HSTRING) { return S_OK; }
	HRESULT __stdcall Run()
	{
		bool develMode = false;
		bool debugWait = false;
		//foreach(const QByteArray &arg, m_argv) {
		//	if (arg == "-qdevel")
		//		develMode = true;
		//	if (arg == "-qdebug")
		//		debugWait = true;
		//}
//		if (develMode) {
//			// Write a PID file to help runner
//			const QString pidFileName = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation))
//				.absoluteFilePath(QString::number(uint(GetCurrentProcessId())) + QStringLiteral(".pid"));
//			CREATEFILE2_EXTENDED_PARAMETERS params = {
//				sizeof(CREATEFILE2_EXTENDED_PARAMETERS),
//				FILE_ATTRIBUTE_NORMAL, FILE_FLAG_DELETE_ON_CLOSE
//			};
//			// (Unused) handle will automatically be closed when the app exits
//			CreateFile2(reinterpret_cast<LPCWSTR>(pidFileName.utf16()),
//				0, FILE_SHARE_READ | FILE_SHARE_DELETE, CREATE_ALWAYS, &params);
//			// Install the develMode message handler
//#ifndef Q_OS_WINPHONE
//			defaultMessageHandler = qInstallMessageHandler(devMessageHandler);
//#endif
//		}
//		// Wait for debugger before continuing
//		if (debugWait) {
//			while (!IsDebuggerPresent())
//				WaitForSingleObjectEx(GetCurrentThread(), 1, true);
//		}
//		g_mainExitCode = main(m_argv.count(), m_argv.data());
		g_mainExitCode = main(0, NULL);
		return S_OK;
	}
	HRESULT __stdcall Uninitialize() { return S_OK; }

private:
	// Activation handler
	HRESULT onActivated(Core::ICoreApplicationView *, Activation::IActivatedEventArgs *args)
	{
		//Activation::ILaunchActivatedEventArgs *launchArgs;
		//if (SUCCEEDED(args->QueryInterface(&launchArgs))) {
		//	for (int i = m_argc; i < m_argv.size(); ++i)
		//		delete[] m_argv[i];
		//	m_argv.resize(m_argc);
		//	HString arguments;
		//	launchArgs->get_Arguments(arguments.GetAddressOf());
			//if (arguments.IsValid()) {
			//	foreach(const QByteArray &arg, QString::fromWCharArray(
			//		arguments.GetRawBuffer(nullptr)).toLocal8Bit().split(' ')) {
			//		m_argv.append(qstrdup(arg.constData()));
			//	}
			//}
		//}
		return S_OK;
	}

	int m_argc;
	//QVector<char *> m_argv;
	char** m_argv;
	bool m_deleteArgv0;
	EventRegistrationToken m_activationToken;
};

class AppViewSource : public Microsoft::WRL::RuntimeClass<Core::IFrameworkViewSource>
{
public:
	AppViewSource(int argc, char **argv) : m_argc(argc), m_argv(argv) { }
	HRESULT __stdcall CreateView(Core::IFrameworkView **frameworkView)
	{
		return (*frameworkView = Make<AppContainer>(m_argc, m_argv).Detach()) ? S_OK : E_OUTOFMEMORY;
	}
private:
	int m_argc;
	char **m_argv;
};

// Main entry point for Appx containers
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#if _MSC_VER < 1900
	int argc = 0;
	char **argv, **env;
	_startupinfo info = { _query_new_mode() };
	if (int init = __getmainargs(&argc, &argv, &env, false, &info))
		return init;
#else
	//QByteArray commandLine = QString::fromWCharArray(GetCommandLine()).toUtf8();
	//QVarLengthArray<char *> args;
	//args.append(commandLine.data());
	//bool quote = false;
	//bool escape = false;
	//for (int i = 0; i < commandLine.size(); ++i) {
	//	switch (commandLine.at(i)) {
	//	case '\\':
	//		escape = true;
	//		break;
	//	case '"':
	//		if (escape) {
	//			escape = false;
	//			break;
	//		}
	//		quote = !quote;
	//		commandLine[i] = '\0';
	//		break;
	//	case ' ':
	//		if (quote)
	//			break;
	//		commandLine[i] = '\0';
	//		if (args.last()[0] != '\0')
	//			args.append(commandLine.data() + i + 1);
	//		// fall through
	//	default:
	//		if (args.last()[0] == '\0')
	//			args.last() = commandLine.data() + i;
	//		escape = false; // only quotes are escaped
	//		break;
	//	}
	//}
	//int argc = args.size();
	//WPB
	int argc = 0;
	//char **argv = args.data();
	//WPB
	char **argv = NULL;

	//char **env = Q_NULLPTR;
#endif // _MSC_VER >= 1900

	//for (int i = 0; env && env[i]; ++i) {
	//	QByteArray var(env[i]);
	//	int split = var.indexOf('=');
	//	if (split > 0)
	//		qputenv(var.mid(0, split), var.mid(split + 1));
	//}

	if (FAILED(RoInitialize(RO_INIT_MULTITHREADED)))
		return 1;

	Core::ICoreApplication *appFactory;
	if (FAILED(RoGetActivationFactory(qHString(CoreApplicationClass), IID_PPV_ARGS(&appFactory))))
		return 2;

	appFactory->Run(Make<AppViewSource>(argc, argv).Get());
	return g_mainExitCode;
}