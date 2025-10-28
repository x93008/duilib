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
    
    // 文本颜色设置（用于子控件）
    void SetNormalTextColor(DWORD dwColor);
    void SetHotTextColor(DWORD dwColor);
    void SetPushedTextColor(DWORD dwColor);
    void SetDisabledTextColor(DWORD dwColor);
    
    // 按钮状态获取（重写基类虚方法）
    virtual bool IsHot() const override { return m_bHot; }
    virtual bool IsPushed() const override { return m_bPushed; }
    
protected:
    virtual void PaintText(HDC hDC);
    void UpdateChildrenState();
    void UpdateContainerTextColor(CContainerUI* pContainer, DWORD dwColor);
    DWORD ParseColor(LPCTSTR pstrColor);
    
protected:
    // 按钮状态
    bool m_bHot;
    bool m_bPushed;
    bool m_bFocused;
    
    // 文字颜色（用于子控件）
    DWORD m_dwNormalTextColor;
    DWORD m_dwHotTextColor;
    DWORD m_dwPushedTextColor;
    DWORD m_dwDisabledTextColor;
};

}	// namespace DuiLib

#endif // __UICONTAINERBUTTON_H__