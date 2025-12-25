#ifndef RotateImageUI_h__
#define RotateImageUI_h__

#pragma once

namespace DuiLib
{
	class CControl;

#define ROTATE_IMAGE_TIMER_ID	101

	class DUILIB_API CRotateImageUI : public CControlUI
	{
	public:
		CRotateImageUI(void);
		~CRotateImageUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoInit() override;
		bool	DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void	DoEvent(TEventUI& event);
		void	SetVisible(bool bVisible = true);
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		// 图片相关
		void	SetBkImage(LPCTSTR pStrImage);
		LPCTSTR GetBkImage();

		// 旋转控制
		void	SetRotationSpeed(float fSpeed);		// 度/秒
		float	GetRotationSpeed() const;
		void	SetAngle(float fAngle);
		float	GetAngle() const;
		void	StartRotation();
		void	StopRotation();
		bool    IsRotating() const;

	private:
		void	SetAutoPlay(bool bIsAuto);
		void	InitImage();
		void	DeleteImage();
		void	OnTimer(UINT_PTR idEvent);
		void	DrawRotatedImage(HDC hDC);
		Gdiplus::Image*	LoadImageFromFile(LPCTSTR pstrImagePath);
		Gdiplus::Image* LoadImageFromMemory(LPVOID pBuf, size_t dwSize);

	private:
		Gdiplus::Image*	m_pImage;
		CDuiString		m_sBkImage;
		float			m_fAngle;				// 当前角度（度）
		float			m_fRotationSpeed;		// 旋转速度（度/秒）
		bool			m_bIsAutoPlay;			// 是否自动旋转
		bool			m_bIsRotating;			// 是否正在旋转
		bool			m_bRotationEnabled;		// 是否启用旋转
		bool			m_needStartWhenShow;	// 当show时，是否需要启动旋转
		IStream*		m_pStream;
	};

}

#endif // RotateImageUI_h__

