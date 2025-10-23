#ifndef __UICONTAINERBUTTON_H__
#define __UICONTAINERBUTTON_H__

#pragma once

namespace DuiLib
{
	class CContainerButtonUI : public CContainerUI
{
public:
    CContainerButtonUI();
    
    virtual LPCTSTR GetClass() const override;
    virtual LPVOID GetInterface(LPCTSTR pstrName) override;
    virtual UINT GetControlFlags() const override;
    
    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
    virtual void DoEvent(TEventUI& event) override;
    virtual bool Activate() override;
    
    // 按钮状态设置
    void SetNormalBkColor(DWORD dwColor);
    void SetHotBkColor(DWORD dwColor);
    void SetPushedBkColor(DWORD dwColor);
    void SetDisabledBkColor(DWORD dwColor);
    
    void SetNormalTextColor(DWORD dwColor);
    void SetHotTextColor(DWORD dwColor);
    void SetPushedTextColor(DWORD dwColor);
    void SetDisabledTextColor(DWORD dwColor);
    
    // 按钮状态获取（重写基类虚方法）
    virtual bool IsHot() const override { return m_bHot; }
    virtual bool IsPushed() const override { return m_bPushed; }
    
protected:
    virtual void PaintBkColor(HDC hDC);
    virtual void PaintText(HDC hDC);
    void UpdateChildrenState();
    void UpdateContainerTextColor(CContainerUI* pContainer, DWORD dwColor);
    DWORD ParseColor(LPCTSTR pstrColor);
    
protected:
    // 按钮状态
    bool m_bHot;
    bool m_bPushed;
    bool m_bFocused;
    
    // 背景颜色
    DWORD m_dwNormalBkColor;
    DWORD m_dwHotBkColor;
    DWORD m_dwPushedBkColor;
    DWORD m_dwDisabledBkColor;
    
    // 文字颜色
    DWORD m_dwNormalTextColor;
    DWORD m_dwHotTextColor;
    DWORD m_dwPushedTextColor;
    DWORD m_dwDisabledTextColor;
    
    // 缓存原始颜色（用于恢复）
    CDuiPtrArray m_OriginalTextColors;
    CDuiPtrArray m_OriginalBkColors;
};

}	// namespace DuiLib

#endif // __UICONTAINERBUTTON_H__