#include "pch.h"
#include "Window.hpp"

static constexpr auto g_className = L"WindowClass";
static const auto g_hInstance = GetModuleHandleW( nullptr );

Window::Window( const wchar_t* title, UINT width, UINT height ) :
	m_width( width ),
	m_height( height )
{
	WNDCLASSEX wc = {
		.cbSize = sizeof( WNDCLASSEX ),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = HandleMsgSetup,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = g_hInstance,
		.hIcon = nullptr,
		.hCursor = LoadCursorW( nullptr, IDC_ARROW ),
		.hbrBackground = nullptr,
		.lpszMenuName = nullptr,
		.lpszClassName = g_className,
		.hIconSm = nullptr,
	};

	RegisterClassExW( &wc );

	RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	m_hwnd = CreateWindowExW( 0u, // WS_EX_TOPMOST, // Just to prevent the console messing up my initial mouse pos
							  g_className, title,
							  WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, CW_USEDEFAULT,
							  rc.right - rc.left,
							  rc.bottom - rc.top,
							  nullptr,
							  nullptr,
							  g_hInstance,
							  this );

	assert( m_hwnd );

	ShowWindow( m_hwnd, SW_SHOW );
	UpdateWindow( m_hwnd );
}

Window::~Window()
{
	if( m_hwnd )
	{
		DestroyWindow( m_hwnd );
	}
	UnregisterClassW( g_className, g_hInstance );
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	while( PeekMessageW( &msg, nullptr, 0u, 0u, PM_REMOVE ) )
	{
		if( msg.message == WM_QUIT )
		{
			return static_cast<int>(msg.wParam);
		}

		TranslateMessage( &msg );
		DispatchMessageW( &msg );
	}

	return std::nullopt;
}

LRESULT Window::HandleMsgSetup( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	if( msg == WM_NCCREATE )
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtrW( hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow) );
		SetWindowLongPtrW( hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk) );

		return pWindow->HandleMsg( hwnd, msg, wParam, lParam );
	}

	return DefWindowProcW( hwnd, msg, wParam, lParam );
}

LRESULT Window::HandleMsgThunk( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtrW( hwnd, GWLP_USERDATA ));

	if( pWindow )
	{
		return pWindow->HandleMsg( hwnd, msg, wParam, lParam );
	}

	return DefWindowProcW( hwnd, msg, wParam, lParam );
}

LRESULT Window::HandleMsg( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	switch( msg )
	{
		case WM_KEYDOWN:	 m_input.OnKeyDown( static_cast<uint8_t>(wParam) ); break;
		case WM_KEYUP:		 m_input.OnKeyUp( static_cast<uint8_t>(wParam) ); break;

		case WM_SYSKEYDOWN:	 m_input.OnKeyDown( static_cast<uint8_t>(wParam) );
			return DefWindowProcW( hwnd, msg, wParam, lParam );
		case WM_SYSKEYUP:    m_input.OnKeyUp( static_cast<uint8_t>(wParam) );
			return DefWindowProcW( hwnd, msg, wParam, lParam );

		case WM_MOUSEMOVE:	 m_input.OnMouseMove( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) ); break;
		case WM_LBUTTONDOWN: m_input.OnMouseDown( MouseButton::Left ); break;
		case WM_LBUTTONUP:   m_input.OnMouseUp( MouseButton::Left ); break;
		case WM_RBUTTONDOWN: m_input.OnMouseDown( MouseButton::Right ); break;
		case WM_RBUTTONUP:   m_input.OnMouseUp( MouseButton::Right ); break;
		case WM_MOUSEWHEEL:  m_input.OnMouseWheel( GET_WHEEL_DELTA_WPARAM( wParam ) ); break;

		case WM_DESTROY:     PostQuitMessage( 0 ); break;

		default:
			return DefWindowProcW( hwnd, msg, wParam, lParam );
	}
	return 0;
}
