#include "StdAfx.h"
#include "UIRotateImage.h"

///////////////////////////////////////////////////////////////////////////////////////
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{
	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);
///////////////////////////////////////////////////////////////////////////////////////

namespace DuiLib
{

	CRotateImageUI::CRotateImageUI(void)
	{
		m_pImage			= NULL;
		m_fAngle			= 0.0f;
		m_fRotationSpeed	= 90.0f;		// 默认90度/秒
		m_bIsAutoPlay		= true;
		m_bIsRotating		= false;
		m_needStartWhenShow	= false;
		m_pStream			= NULL;
	}

	CRotateImageUI::~CRotateImageUI(void)
	{
		DeleteImage();
		m_pManager->KillTimer(this, ROTATE_IMAGE_TIMER_ID);
	}

	LPCTSTR CRotateImageUI::GetClass() const
	{
		return DUI_CTR_ROTATEIMAGE;
	}

	LPVOID CRotateImageUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcscmp(pstrName, DUI_CTR_ROTATEIMAGE) == 0) return static_cast<CRotateImageUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CRotateImageUI::DoInit()
	{
		InitImage();
	}

	bool CRotateImageUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		if (NULL == m_pImage)
		{
			InitImage();
		}
		DrawRotatedImage(hDC);
		return true;
	}

	void CRotateImageUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_TIMER)
			OnTimer((UINT_PTR)event.wParam);
		CControlUI::DoEvent(event);
	}

	void CRotateImageUI::SetVisible(bool bVisible /* = true */)
	{
		CControlUI::SetVisible(bVisible);
		if (!bVisible && m_bIsRotating) {
			// 因为隐藏导致的结束旋转，需要在下一次show时重新启动旋转
			StopRotation();
			m_needStartWhenShow = true;
		}
		else if (bVisible && m_needStartWhenShow) {
			m_needStartWhenShow = false;
			StartRotation();
		}
	}

	void CRotateImageUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("bkimage")) == 0) SetBkImage(pstrValue);
		else if (_tcscmp(pstrName, _T("rotationspeed")) == 0) {
			SetRotationSpeed((float)_ttof(pstrValue));
		}
		else if (_tcscmp(pstrName, _T("autoplay")) == 0) {
			SetAutoPlay(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if (_tcscmp(pstrName, _T("angle")) == 0) {
			SetAngle((float)_ttof(pstrValue));
		}
		else
			CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CRotateImageUI::SetBkImage(LPCTSTR pStrImage)
	{
		if (m_sBkImage == pStrImage || NULL == pStrImage) return;

		bool bWasRotating = m_bIsRotating;
		StopRotation();
		DeleteImage();

		m_sBkImage = pStrImage;
		Invalidate();

		// 如果之前正在旋转，重新启动旋转
		if (bWasRotating)
		{
			InitImage();
			if (m_pImage)
				StartRotation();
		}
	}

	LPCTSTR CRotateImageUI::GetBkImage()
	{
		return m_sBkImage.GetData();
	}

	void CRotateImageUI::SetRotationSpeed(float fSpeed)
	{
		m_fRotationSpeed = fSpeed;
		// 如果正在旋转，重新启动以应用新速度
		if (m_bIsRotating)
		{
			StopRotation();
			StartRotation();
		}
	}

	float CRotateImageUI::GetRotationSpeed() const
	{
		return m_fRotationSpeed;
	}

	void CRotateImageUI::SetAutoPlay(bool bIsAuto)
	{
		m_bIsAutoPlay = bIsAuto;
		// SetAutoPlay 仅作为标记，不自动启动/停止旋转
		// 实际的启动/停止由 StartRotation/StopRotation 控制
	}

	void CRotateImageUI::SetAngle(float fAngle)
	{
		m_fAngle = fAngle;
		// 规范化角度到 0-360 度
		while (m_fAngle >= 360.0f) m_fAngle -= 360.0f;
		while (m_fAngle < 0.0f) m_fAngle += 360.0f;
		Invalidate();
	}

	float CRotateImageUI::GetAngle() const
	{
		return m_fAngle;
	}

	void CRotateImageUI::StartRotation()
	{
		if (m_bIsRotating || m_pImage == NULL) return;

		// 计算定时器间隔（约30fps，33ms）
		UINT nInterval = 33;
		m_pManager->SetTimer(this, ROTATE_IMAGE_TIMER_ID, nInterval);
		m_bIsRotating = true;
	}

	void CRotateImageUI::StopRotation()
	{
		if (!m_bIsRotating) return;

		m_pManager->KillTimer(this, ROTATE_IMAGE_TIMER_ID);
		m_bIsRotating = false;
		m_needStartWhenShow = false;
	}

	bool CRotateImageUI::IsRotating() const
	{
		return m_bIsRotating;
	}

	void CRotateImageUI::InitImage()
	{
		m_pImage = LoadImageFromFile(GetBkImage());
		if (NULL == m_pImage) return;

		// 如果设置了 autoplay 属性，自动启动旋转
		if (m_bIsAutoPlay && IsVisible())
		{
			StartRotation();
		}
	}

	void CRotateImageUI::DeleteImage()
	{
		if (m_pStream != NULL)
		{
			m_pStream->Release();
			m_pStream = NULL;
		}
		if (m_pImage != NULL)
		{
			delete m_pImage;
			m_pImage = NULL;
		}
	}

	void CRotateImageUI::OnTimer(UINT_PTR idEvent)
	{
		if (idEvent != ROTATE_IMAGE_TIMER_ID)
			return;

		// 更新角度（每帧增加的角度 = 速度 * 时间间隔）
		// 定时器间隔约33ms，即约0.033秒
		float fDeltaAngle = m_fRotationSpeed * 0.033f;
		m_fAngle += fDeltaAngle;

		// 规范化角度到 0-360 度
		while (m_fAngle >= 360.0f) m_fAngle -= 360.0f;

		Invalidate();
	}

	void CRotateImageUI::DrawRotatedImage(HDC hDC)
	{
		if (NULL == hDC || NULL == m_pImage) return;

		// 获取控件区域
		RECT rcItem = m_rcItem;
		int width = rcItem.right - rcItem.left;
		int height = rcItem.bottom - rcItem.top;

		if (width <= 0 || height <= 0) return;

		// 计算中心点
		int centerX = rcItem.left + width / 2;
		int centerY = rcItem.top + height / 2;

		// 创建Graphics对象
		Gdiplus::Graphics graphics(hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
		graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

		// 检查是否有圆角设置
		SIZE borderRound = GetBorderRound();
		if (borderRound.cx > 0 || borderRound.cy > 0)
		{
			// 创建圆角路径
			Gdiplus::GraphicsPath path;
			int radiusX = borderRound.cx;
			int radiusY = borderRound.cy;

			// 确保圆角不超过控件大小的一半
			if (radiusX * 2 > width) radiusX = width / 2;
			if (radiusY * 2 > height) radiusY = height / 2;

			// 创建圆角矩形路径（从左上角开始，顺时针）
			float left = (float)rcItem.left;
			float top = (float)rcItem.top;
			float right = (float)rcItem.right;
			float bottom = (float)rcItem.bottom;
			float diameterX = (float)(radiusX * 2);
			float diameterY = (float)(radiusY * 2);

			// 左上角
			path.AddArc(left, top, diameterX, diameterY, 180, 90);
			// 右上角
			path.AddArc(right - diameterX, top, diameterX, diameterY, 270, 90);
			// 右下角
			path.AddArc(right - diameterX, bottom - diameterY, diameterX, diameterY, 0, 90);
			// 左下角
			path.AddArc(left, bottom - diameterY, diameterX, diameterY, 90, 90);
			path.CloseFigure();

			// 设置裁剪区域
			graphics.SetClip(&path);
		}

		// 保存当前状态
		Gdiplus::GraphicsState state = graphics.Save();

		// 设置变换：移动到中心点 -> 旋转 -> 移回
		graphics.TranslateTransform((Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY);
		graphics.RotateTransform(m_fAngle);
		graphics.TranslateTransform((Gdiplus::REAL)(-width / 2.0f), (Gdiplus::REAL)(-height / 2.0f));

		// 绘制图片（自动缩放适应控件大小）
		graphics.DrawImage(m_pImage, 0.0f, 0.0f, (Gdiplus::REAL)width, (Gdiplus::REAL)height);

		// 恢复状态
		graphics.Restore(state);

		// 恢复裁剪区域
		graphics.ResetClip();
	}

	Gdiplus::Image* CRotateImageUI::LoadImageFromFile(LPCTSTR pstrImagePath)
	{
		LPBYTE pData = NULL;
		DWORD dwSize = 0;

		do
		{
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			if (CPaintManagerUI::GetResourceZip().IsEmpty()) {
				sFile += pstrImagePath;
				HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE) break;
				dwSize = ::GetFileSize(hFile, NULL);
				if (dwSize == 0)
				{
					::CloseHandle(hFile);
					break;
				}

				DWORD dwRead = 0;
				pData = new BYTE[dwSize];
				::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
				::CloseHandle(hFile);

				if (dwRead != dwSize) {
					delete[] pData;
					pData = NULL;
					break;
				}
			}
			else {
				sFile += CPaintManagerUI::GetResourceZip();
				HZIP hz = NULL;
				if (CPaintManagerUI::IsCachedResourceZip()) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
				else hz = OpenZip((void*)sFile.GetData(), 0, 2);
				if (hz == NULL) break;
				ZIPENTRY ze;
				int i;
				if (FindZipItem(hz, pstrImagePath, true, &i, &ze) != 0) break;
				dwSize = ze.unc_size;
				if (dwSize == 0) break;
				pData = new BYTE[dwSize];
				int res = UnzipItem(hz, i, pData, dwSize, 3);
				if (res != 0x00000000 && res != 0x00000600) {
					delete[] pData;
					pData = NULL;
					if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
					break;
				}
				if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
			}

		} while (0);

		while (!pData)
		{
			//读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
			HANDLE hFile = ::CreateFile(pstrImagePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) break;
			dwSize = ::GetFileSize(hFile, NULL);
			if (dwSize == 0)
			{
				::CloseHandle(hFile);
				break;
			}

			DWORD dwRead = 0;
			pData = new BYTE[dwSize];
			::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
			::CloseHandle(hFile);

			if (dwRead != dwSize) {
				delete[] pData;
				pData = NULL;
			}
			break;
		}
		if (!pData)
		{
			return NULL;
		}

		Gdiplus::Image* pImage = LoadImageFromMemory(pData, dwSize);
		delete[] pData;
		return pImage;
	}

	Gdiplus::Image* CRotateImageUI::LoadImageFromMemory(LPVOID pBuf, size_t dwSize)
	{
		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
		BYTE* pMem = (BYTE*)::GlobalLock(hMem);

		memcpy(pMem, pBuf, dwSize);
		::GlobalUnlock(hMem);

		::CreateStreamOnHGlobal(hMem, TRUE, &m_pStream);
		Gdiplus::Image *pImg = Gdiplus::Image::FromStream(m_pStream);
		if (!pImg || pImg->GetLastStatus() != Gdiplus::Ok)
		{
			m_pStream->Release();
			m_pStream = NULL;
			return 0;
		}
		return pImg;
	}

}

