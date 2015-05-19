#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "Singleton.h"
#include "RenderManager.h"
#include "DataTypes.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

namespace KVE
{
	namespace System
	{
		typedef struct WindowParams
		{
			HINSTANCE	HInstance;
			u16			Width;
			u16			Height;
			LPCWSTR		Name;
			WNDPROC		WndProcedure;
		} WindowParams;

		class Window
		{
		public:
			const u16 width( void ) const;
			const u16 height( void ) const;
			const f32 aspectRatio( void ) const;
			const bool isMinimized( void ) const;
			const bool isMaximized( void ) const;
			const bool isResizing( void ) const;
			const bool isPaused( void ) const;
			const HWND* const getHandle( void ) const;

			void setWidth( u16 width );
			void setHeight( u16 height );
			void setMinimized( bool minimized );
			void setMaximized( bool maximized );
			void setResizing( bool resizing );
			void setPaused( bool paused );

		protected:
			Window( const WindowParams& params );

			void onResize( u16 width, u16 height );

		protected:
			HWND		m_HWindow;

			u16			m_Width;
			u16			m_Height;
			f32			m_AspectRatio;

			bool		m_Minimized : 1;
			bool		m_Maximized : 1;
			bool		m_Resizing : 1;
			bool		m_Paused : 1;
		};
	}
}

#endif